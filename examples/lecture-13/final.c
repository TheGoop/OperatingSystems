#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_THREADS 4

void *run(void *v)
{

  pid_t pid;
  pid = fork();
  if (pid == 0)
  {
    printf("Hello from child \n");
  }
  else
  {
    printf("Hello from parent\n");
  }
}

int main()
{
  pthread_t threads[NUM_THREADS];
  for (int i = 0; i < NUM_THREADS; ++i)
  {
    pthread_create(&threads[i], NULL, &run, NULL);
  }
  printf("Here \n");
  for (int i = 0; i < NUM_THREADS; ++i)
  {
    pthread_join(threads[i], NULL);
  }
  return 0;
}