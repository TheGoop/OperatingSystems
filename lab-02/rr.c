#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

typedef uint32_t u32;
typedef int32_t i32;

struct process
{
  u32 pid;
  u32 arrival_time;
  u32 burst_time;

  TAILQ_ENTRY(process)
  pointers;

  /* Additional fields here */
  bool has_started;
  u32 remaining_time;
  u32 rr_time;
  /* End of "Additional fields here" */
};

TAILQ_HEAD(process_list, process);

u32 next_int(const char **data, const char *data_end)
{
  u32 current = 0;
  bool started = false;
  while (*data != data_end)
  {
    char c = **data;

    if (c < 0x30 || c > 0x39)
    {
      if (started)
      {
        return current;
      }
    }
    else
    {
      if (!started)
      {
        current = (c - 0x30);
        started = true;
      }
      else
      {
        current *= 10;
        current += (c - 0x30);
      }
    }

    ++(*data);
  }

  printf("Reached end of file while looking for another integer\n");
  exit(EINVAL);
}

u32 next_int_from_c_str(const char *data)
{
  char c;
  u32 i = 0;
  u32 current = 0;
  bool started = false;
  while ((c = data[i++]))
  {
    if (c < 0x30 || c > 0x39)
    {
      exit(EINVAL);
    }
    if (!started)
    {
      current = (c - 0x30);
      started = true;
    }
    else
    {
      current *= 10;
      current += (c - 0x30);
    }
  }
  return current;
}

void init_processes(const char *path,
                    struct process **process_data,
                    u32 *process_size)
{
  int fd = open(path, O_RDONLY);
  if (fd == -1)
  {
    int err = errno;
    perror("open");
    exit(err);
  }

  struct stat st;
  if (fstat(fd, &st) == -1)
  {
    int err = errno;
    perror("stat");
    exit(err);
  }

  u32 size = st.st_size;
  const char *data_start = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (data_start == MAP_FAILED)
  {
    int err = errno;
    perror("mmap");
    exit(err);
  }

  const char *data_end = data_start + size;
  const char *data = data_start;

  *process_size = next_int(&data, data_end);

  *process_data = calloc(sizeof(struct process), *process_size);
  if (*process_data == NULL)
  {
    int err = errno;
    perror("calloc");
    exit(err);
  }

  for (u32 i = 0; i < *process_size; ++i)
  {
    (*process_data)[i].pid = next_int(&data, data_end);
    (*process_data)[i].arrival_time = next_int(&data, data_end);
    (*process_data)[i].burst_time = next_int(&data, data_end);
  }

  munmap((void *)data, size);
  close(fd);
}

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    return EINVAL;
  }
  struct process *data;
  u32 size;
  init_processes(argv[1], &data, &size);

  u32 quantum_length = next_int_from_c_str(argv[2]);

  struct process_list list; //list of every process that is active/has to run
  TAILQ_INIT(&list);

  u32 total_waiting_time = 0;
  u32 total_response_time = 0;

  /* Your code here */
  // set all of the processes' remaining times
  struct process *p;
  for (u32 i = 0; i < size; i++)
  {
    p = &data[i];
    p->remaining_time = p->burst_time;
    p->has_started = false;
    p->rr_time = quantum_length;
  }

  u32 t = 0;
  bool finished = false;
  while (!finished)
  {
    //load any new processes into the list
    for (u32 i = 0; i < size; i++)
    {
      p = &data[i];
      if (p->arrival_time == t)
      {
        p->remaining_time = p->burst_time;
        p->has_started = false;
        p->rr_time = quantum_length;
        TAILQ_INSERT_TAIL(&list, p, pointers);
        //we insert at the tail, because newly recieved processes go to the end of the queue not front
        printf("%d arrived at %d with %d time to burst\n", p->pid, p->arrival_time, p->burst_time);
      }
    }

    if (!TAILQ_EMPTY(&list))
    {
      p = TAILQ_FIRST(&list);

      // if this hasn't recieved a response yet, update the total response time
      if (p->has_started == false)
      {
        // printf("Process %d recieved response at %d. Response Time: %d\n", p->pid, t, t - p->arrival_time);
        total_response_time += t - p->arrival_time;
        p->has_started = true;
      }

      //"run" p
      p->rr_time -= 1;
      p->remaining_time -= 1;

      // if p is done running
      if (p->remaining_time <= 0)
      {
        total_waiting_time += t - (p->burst_time + p->arrival_time) + 1;
        // printf("Process %d - Wait Time: %d\n", p->pid, t - (p->burst_time + p->arrival_time));
        // printf("Arrival: %d, Burst: %d, Finished: %d \n", p->arrival_time, p->burst_time, t);
        //remove from list of processes
        TAILQ_REMOVE(&list, p, pointers);
      }

      // if p doesn't have time left in its rr turn
      if (p->rr_time <= 0)
      {
        p->rr_time = quantum_length; //reset its turn timer
        //move p to back of the queue
        TAILQ_REMOVE(&list, p, pointers);
        TAILQ_INSERT_TAIL(&list, p, pointers);
      }
    }

    t += 1;

    // if (t > 15)
    // {
    //   break;
    // }
  }

  /* End of "Your code here" */

  printf("Average waiting time: %.2f\n", (float)total_waiting_time / (float)size);
  printf("Average response time: %.2f\n", (float)total_response_time / (float)size);

  free(data);
  return 0;
}
