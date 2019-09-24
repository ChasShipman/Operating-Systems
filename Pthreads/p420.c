#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include "p320.h"


void *threadRunner();

int main(int argc, char **argv) {
    if(argc <  2){
        printf("Error:\n\tUsage: ./p420 numberOfThreads\n");
        exit(1);
    }
    
    int numThreads = atoi(argv[1]);//read command line arg
	
    srand(time(NULL));
	pthread_t threads[numThreads];
	int i = 0;
	int return_status;

	return_status = allocate_map();	//create the bitmap pid manager
	if(return_status == 0) {//error check
		printf("ERROR: Bitmap not sucessfully created\n");
	}
    else printf("Bitmap successfully created!\n\n");

	printf("\n\n---Creating Threads---\n");
	for(i = 0; i < numThreads; i++) {
		pthread_create(&threads[i],0,threadRunner,NULL);//create threads
		usleep(75000);
	}
	printf("\n\n---Joining Threads---\n"); //join threads
	for(i = 0; i < numThreads; i++) {
		pthread_join(threads[i], NULL);
	}
	return 1;
}

void *threadRunner() {
    int pid = 0;
    pid = allocate_pid();    //allocates pid
    if(pid == 1) {
        printf("No available PID\n");
    }
    else {
        printf("Allocated PID: %d\n", pid);
        int y = rand()%10; //gets a random number in 0-10 range
        sleep(y);    //sleeps for random length number ^
        release_pid(pid);    //releases pid
        printf("Released PID: %d\n", pid);
    }
    return NULL;
}
