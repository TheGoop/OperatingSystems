#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

char *run_process(char *process, char *input)
{
	//Executes and returns the stdout of the process that runs
	//execlp(process, process) and input
	return "";
}
int get(int *fds, int colLength, int r, int c)
{
	return *(fds + r * colLength + c);
}

int main(int argc, char *argv[])
{
	/*
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			printf("Row: %d, Col: %d, Val: %d \n", i, j, *(fds + i * cols + j));
			close(*(fds + i * cols + j));
		}
	}

	int i;
	for (i = 1; i < argc; i++)
		printf("%d: %s\n", i, argv[i]);
	printf("%d\n", argc);
	*/

	int rows = argc - 1; //need a set of fds between every process, and a final set to the parent
	int cols = 2;
	int *fds = (int *)malloc(rows * cols * sizeof(int));
	int i, j;
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			*(fds + i * cols + j) = 0;
			//printf("Row: %d, Col: %d, Val: %d \n", i, j, *(fds + i * cols + j));
		}
	}

	//load pipe file descriptos into fds
	int cpid;
	for (i = 0; i < rows; i++)
	{
		cpid = pipe(fds + i * cols);
		if (cpid == -1)
		{
			fprintf(stderr, "Pipe failed.\n");
			return EXIT_FAILURE;
		}
	}

	//set the last file descriptor to 1 so that the last process prints the output directly
	*(fds + (rows - 1) * cols + cols - 1) = 1;

	int readfd = 0;
	int writefd = 1;

	pid_t pid;
	//spawn the processes
	for (i = 1; i < argc; i++)
	{
		//set the appropriate fd for this process
		if (i != 1)
		{
			readfd = get(fds, cols, i - 2, 0);
		}
		writefd = get(fds, cols, i - 1, 1);
		//printf("Command: %s, Read: %d, Write: %d \n", argv[i], readfd, writefd);

		//fork
		pid = fork();

		//if child:
		if (pid == (pid_t)0)
		{
			//changes the read, write fds then calls the program to be run
			dup2(readfd, 0);
			dup2(writefd, 1);

			if (execlp(argv[i], argv[i]) == -1)
			{
				printf("Fail.");
				printf("%s failed with errno %d", argv[i], errno);
			}
		}

		// else if fork failed
		else if (pid < (pid_t)0)
		{
			fprintf(stderr, "Fork failed.\n");
			return EXIT_FAILURE;
		}

		//else if parent:
		else
		{
			//do nothing, keep going
			close(readfd);
			close(writefd);
		}
	}

	/* Wait for children to exit. */
	int n = argc - 1; //number of commands aka processes because argc is 1 more than number of args
	int status;
	while (n > 0)
	{
		pid = wait(&status);
		printf("Child with PID %ld exited with status 0x%x.\n", (long)pid, status);
		--n; // TODO(pts): Remove pid from the pids array.
	}

	return 0;
}

//ls   read      write 0, 1
//cat  read 0, 0 write 1, 1
//wc   read 1, 0 write 2, 1