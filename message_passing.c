#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define READ_END 0
#define WRITE_END 1

int main() {

	int fd[2];
	int fdd[2];

	pid_t pid;

	// create the pipe, so it can be shared
	// between the parent and the child
	if(pipe(fd) == -1) {
		printf("ERROR: pipe1 creation failed.\n");
		exit(1);
	}

	// create a second pipe for returning value
	if(pipe(fdd) == -1) {
		printf("ERROR: pipe2 creation failed.\n");
		exit(1);
	}

	// create a process
	pid = fork();

	if(pid < 0) {

		printf("ERROR: fork failed.\n");

	} else if (pid == 0) {

		// inside child process

		int* parent_int = (int *)malloc(sizeof(int));

		// close the write_end, since it is not needed
		close(fd[WRITE_END]);
		close(fdd[READ_END]);

		// read the integer
		int ret = read(fd[READ_END], parent_int, sizeof(int)); 	
		

		// close the read end
		close(fd[READ_END]);
		
		// display the integer
		printf("Initial number is %d\n", *parent_int);

		// write the squared num back to the parent
		(*parent_int) *= (*parent_int);
		write(fdd[WRITE_END], parent_int, sizeof(int)); 	
		
		// close the write end
		close(fdd[WRITE_END]);

	} else {
		
		// inside parent process

		// generate random number
		int rand_int;

		unsigned int seed = 12445644;
		srand(seed);
		rand_int = rand();

		// close the read_end, since it is not needed
		close(fd[READ_END]);
		close(fdd[WRITE_END]);	

		// now write the integer value
		write(fd[WRITE_END], &rand_int, sizeof(int)); 	

		// close the write end
		close(fd[WRITE_END]);	

		// read the squared num from child
		int* child_int = (int *)malloc(sizeof(int));
		int ret = read(fdd[READ_END], child_int, sizeof(int)); 	
		
		// display the squared num
		printf("Square of %d is %d\n", rand_int, *child_int);

		// close the read handle
		close(fdd[READ_END]);	

		// wait for the child
		wait(NULL);

	}

	

	return 0;

}
