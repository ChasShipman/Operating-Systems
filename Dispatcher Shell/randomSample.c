#include <time.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct process {
	int arrival;
	int priority;
	int burst;
}PROCESS;

void swap(PROCESS *x, PROCESS *y) {
	PROCESS temp;
	temp.arrival = x->arrival;
	temp.priority = x->priority;
	temp.burst = x->burst;

	x->arrival = y->arrival;
	x->priority = y->priority;
	x->burst = y->burst;

	y->arrival = temp.arrival;
	y->priority = temp.priority;
	y->burst = temp.burst;
}

// A function to implement bubble sort
void bubbleSort(PROCESS *arr[], int n) {
	int i, j;
	for (i = 0; i < n-1; i++)
		// Last i elements are already in place
		for (j = 0; j < n-i-1; j++)
			if (arr[j]->arrival > arr[j+1]->arrival)
				swap(arr[j], arr[j+1]);
}

int main(void) {
	time_t t;
	srand((unsigned) time(&t));
	FILE *fp = fopen("rsample.txt", "w+");

	if (fp == NULL) {
		fprintf(stderr, "Error opening file.\n");
		exit(-1);
	}
	int numProcesses = rand() % 1000 + 1;

	PROCESS **p = (PROCESS **)malloc(sizeof(PROCESS *) * numProcesses);
	for (int i = 0; i < numProcesses; i++) {
		p[i] = (PROCESS *)malloc(sizeof(PROCESS));
	}

	for (int i = 0; i < numProcesses; i++) {
		(p[i])->arrival = rand() % 300;
		(p[i])->priority = rand() % 4;
		(p[i])->burst = rand() % 20 + 1;
	}

	bubbleSort(p, numProcesses);
	(p[0])->arrival = 0;

	for (int i = 0; i <= 4; i++) {
		fprintf(fp, "%d, %d, %d\n", p[i]->arrival, p[i]->priority, p[i]->burst);
	}

	return 0;
}
