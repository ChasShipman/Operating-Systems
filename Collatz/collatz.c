#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>



int main(int argc, char **argv) {
	pid_t pid;
    
	int input = atoi(argv[argc - 1]);
	if(input <= 0) { // invalid input
		fprintf(stderr, "Not positive input\n");
		return 1;
	}
	//fork a child process
	pid = fork();
	if (pid < 0) { //error
		fprintf(stderr, "Fork Failed");
		return 1;
	}
	else if (pid == 0) { //child process
		fprintf(stdout, "%d", input);
		//Collatz algorithm
		while(input != 1) {
			if(input % 2 == 0) {
				input = input/2;
			}
			else {	//odd
				input = input * 3 + 1;
			}
		fprintf(stdout, ", %d", input);
		}
		fprintf(stdout,"\n");
	}
	else { //parent process
		wait(NULL); //parent waits for child to finish
	}
	return 0;
}
