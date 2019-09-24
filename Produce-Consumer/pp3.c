#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include "buffer.h"
#include "p320.h"

pthread_t tid;
buffer_item buffer[BUFFER_SIZE];
int counter;
pthread_mutex_t mutex;
pthread_attr_t attr;
sem_t full;
sem_t empty;
void *producer(void *param);
void *consumer(void *param);

int main(int argc, char **argv) {

	//get command line arguements
	int timeSleep = atoi(argv[1]);
	int threadsProd = atoi(argv[2]);
	int threadsCon = atoi(argv[3]);


	//initialize buffer,
	initialize();
	allocate_map();
	//srand(time(NULL));
    //initialize mutex lock

	printf("-----Creating Producers-----\n");
	int i = 0;
	for(i = 0; i < threadsProd; i++) {
		pthread_create(&tid,&attr,producer,NULL);
		usleep(1);
	}

	printf("-----Creating Consumers-----\n");
	for(i = 0; i < threadsCon; i++) {
		pthread_create(&tid,&attr,consumer,NULL);
		usleep(1);
	}
	sleep(timeSleep);
	return 0;
}


void *producer(void *param) {
    buffer_item item;
    pthread_mutex_lock(&mutex);
    int tpid = allocate_pid();
    pthread_mutex_unlock(&mutex);
    while(1) {
        //sleep for a random period of time;
        int r = rand()%10 + 1;
        sleep(r);
        
        item = rand()%999 + 1;
        if(insert_item(item)) {
            fprintf(stderr, "ERROR: Item not inserted correctly\n");
        }
        else {
            printf("Thread ID: %d Producing item: %d\n",tpid,item);
        }
    }
    pthread_mutex_lock(&mutex);
    release_pid(tpid);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void *consumer(void *param) {
    buffer_item item;
    pthread_mutex_lock(&mutex);
    int tpid = allocate_pid();
    pthread_mutex_unlock(&mutex);
    while(1) {
        //sleep for a random period of time;
        int r = rand()%9 + 1;
        sleep(r);
        
        //get item from remove item
        if(remove_item(&item)) {
            fprintf(stderr, "ERROR: Item not removed correctly\n");
        }
        else {
            printf("Thread ID: %d Consuming item: %d\n",tpid,item);
        }
    }
    pthread_mutex_lock(&mutex);
    release_pid(tpid);
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void initialize() { /* initializes buffer, mutex and semaphores*/
    //intialize all variables
    counter = 0;
    sem_init(&full,0,0);
    sem_init(&empty,0,BUFFER_SIZE);
    pthread_mutex_init(&mutex, NULL);
    pthread_attr_init(&attr);
    
    //return 0;
    return;
    
}
int insert_item(buffer_item item) { /* insert item into buffer*/
    
    if(counter < BUFFER_SIZE) {
        buffer[counter] = item;
        counter++;
        return 0;
    }
    else {
        return -1;
    }
    
    
}
int remove_item(buffer_item *item) { /* remove an object from buffer*/
    
    
    if(counter > 0) {
        *item = buffer[(counter - 1)];
        counter--;
        return 0;
    }
    else {
        return -1;
    }
}

