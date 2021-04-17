#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define NAME "/shared-memory-example"
#define SIZE 4096

void report_and_exit(const char *message)
{
  int err = errno;
  perror(message);
  exit(err);
}

void parent()
{
  shm_unlink(NAME);
  int fd = shm_open(NAME, O_CREAT | O_EXCL | O_RDWR, 0600);
  if (fd < 0) {
    report_and_exit("shm_open");
  }

  if (ftruncate(fd, SIZE) == -1) {
    close(fd);
    shm_unlink(NAME);
    report_and_exit("ftruncate");
  }

  void *pointer = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (pointer == MAP_FAILED) {
    close(fd);
    shm_unlink(NAME);
    report_and_exit("mmap");
  }

  int *x = pointer;

  *x = 1;

  sleep(2);

  *x = 2;

  int wstatus;
  wait(&wstatus);

  munmap(pointer, SIZE);
  close(fd);
  shm_unlink(NAME);

  if (!WIFEXITED(wstatus)) {
    printf("child did not make it to exit\n");
    exit(EXIT_FAILURE);
  }

  int child_exit = WEXITSTATUS(wstatus);
  if (child_exit != 0) {
    printf("child did not exit successfully\n");
    exit(child_exit);
  }
}

void child()
{
  sleep(1);

  int fd = shm_open(NAME, O_RDONLY, 0600);
  if (fd < 0) {
    report_and_exit("shm_open");
  }

  void *pointer = mmap(NULL, SIZE, PROT_READ, MAP_SHARED, fd, 0);
  if (pointer == MAP_FAILED) {
    close(fd);
    shm_unlink(NAME);
    report_and_exit("mmap");
  }

  int *x = pointer;

  printf("Read shared memory: %d\n", *x);

  sleep(2);

  printf("Read shared memory: %d\n", *x);

  munmap(pointer, SIZE);
  close(fd);
  shm_unlink(NAME);
}

int main(int argc, char *argv[])
{
  pid_t pid = fork();
  if (pid == -1) {
    report_and_exit("fork");
  }
  if (pid > 0) {
    parent();
  }
  else {
    child();
  }
  return 0;
}
