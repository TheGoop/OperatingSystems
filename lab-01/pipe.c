#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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

	//iterate through every process from left to right 1 -> argc
	//execute each process with the stdin of the stdout of the prev process
	//print to stdout the stoud of the last process to run

	int rows = argc - 1; //need a set of fds between every process, and a final set to the parent
	int cols = 2;
	int *fds = (int *)malloc(rows * cols * sizeof(int));
	int i, j;
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			*(fds + i * cols + j) = 0;
			printf("Row: %d, Col: %d, Val: %d \n", i, j, *(fds + i * cols + j));
		}
	}

	//load pipe file descriptos into fds
	for (i = 0; i < rows; i++)
	{
		pipe(fds + i * cols);
	}

	//spawn the processes
	for (i = 1; i < argc; i++)
	{
		//set the appropriate fd for this process
		//read = [i - 1][0]
		//write = [i][1]
		//fork

		//if child:
		//execlp call to argv[i]

		//else if parent:
		//do nothing, keep going
	}

	//wait for all of the processes to finish

	return 0;
}
