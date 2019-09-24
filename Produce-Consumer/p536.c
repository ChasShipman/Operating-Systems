#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include "p320.h"


void *threadRunner();
pthread_mutex_t mutex;
int counter = 0;
int numThreads = 0;
int main(int argc, char **argv) {
    if(argc <  2){
        printf("Error:\n\tUsage: ./p420 numberOfThreads\n");
        exit(1);
    }
    
    numThreads = atoi(argv[1]);//read command line arg
	
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
        pthread_mutex_init(&mutex, 0);
		pthread_create(&threads[i],0,threadRunner,NULL);//create threads
		usleep(75000);
	}
	printf("\n\n---Joining Threads---\n"); //join threads
	for(i = 0; i < numThreads; i++) {
		pthread_join(threads[i], NULL);
        pthread_mutex_destroy(&mutex);
	}
	return 1;
}

void *threadRunner() {
    int pid = 0;
    
    pthread_mutex_lock(&mutex);
    
    /*----Critical Section----*/
    pid = allocate_pid();    //allocates pid
	counter++;//RACE CONDITION
    
    /*----Release Lock-----*/
    pthread_mutex_unlock(&mutex);

    if(pid == -1) {
        printf("No available PID\n");
    }
    else {
        int y = rand()%10 + 1; //gets a random number in 1-10 range
        printf("Sleeping Time: %d secs ; Thread ID= %d; Counter Value = %d.\n", y, pid, counter - 1);
        sleep(y);    //sleeps for random length number ^
        
        /*---Aquire Lock---*/
        pthread_mutex_lock(&mutex);
        release_pid(pid);    //releases pid
        counter--;//RACE CONDITION
        //printf("Released PID: %d\n", pid);
        printf("Sleeping Time: %d secs ; Thread ID= %d; Counter Value = %d.\n", y, pid, counter);
        
        /*----Release the Lock*/
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}
