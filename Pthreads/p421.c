#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_THREADS 3

int min, max, avg;
void *getMin(void *items);
void *getMax(void *items);
void *getAvg(void *items);

typedef struct number{
	int *nums;
	int len;
}NUMBER;

int main(int argc, char **argv){
	if(argc == 1) {
        printf("ERROR:\n\tUsage: ./p421 arg1 arg2 arg3....argN\n;");
		exit(1);
	}

	NUMBER args;// = malloc(sizeof(args));
	pthread_t workers[NUM_THREADS];
	int *nums = malloc(sizeof(int) * argc - 1);
	int i = 1;
	//evaluation of input and putting in struct
	while(i < argc) {
		nums[i-1] = atoi(argv[i]);
		i++;
	}
	args.nums = nums;
	args.len = argc - 1;
	//create the three threads for statistical evaluation
	pthread_create(&workers[0],0,getAvg,(void *) &args);
	pthread_create(&workers[1],0,getMin,(void *) &args);
	pthread_create(&workers[2],0,getMax,(void *) &args);
	
    i = 0;
	for(i = 0; i < NUM_THREADS; i++){
		pthread_join(workers[i], NULL);
	}

	printf("The avg value is %d\n", avg);
	printf("The min value is %d\n", min);
	printf("The max value is %d\n", max);

	return 1;
}

void *getMin(void *items){
    NUMBER *num = items;
    int i;
    int tempMin = num->nums[0];
    for(i = 1; i < num->len; i++){
        if(tempMin > num->nums[i]){
            tempMin = num->nums[i];
        }
    }
    min = tempMin;
    return NULL;
}

void *getMax(void *items){
    NUMBER *num = items;
    int i;
    int tempMax = num->nums[0];
    for(i = 1; i < num->len; i++){
        if(tempMax < num->nums[i]){
            tempMax = num->nums[i];
        }
    }
    max = tempMax;
    return NULL;
}

void *getAvg(void *items){
    NUMBER *num = items;
    int i;
    double tempAvg;
    int sum;
    for(i = 0; i < num->len; i++){
        sum += num->nums[i];
    }
    tempAvg = sum/(num->len);
    tempAvg = tempAvg;
    avg = (int)tempAvg;
    
    return NULL;
}
