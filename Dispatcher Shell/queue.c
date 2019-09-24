/* 
 *
 *Author: Chas Shipman
 *CS201
 *Assignment 0
 *Anderson
 *Due: 1/30/18
 *
 *
 *This file acts as 
 *an abstract queue
 *data structure built
 *from an underlying
 singly-linked list 
 *
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "queue.h"
#include "sll.h"


struct queue{
	SLL *list;
	void (*display)(void *, FILE *);
	void (*free)(void *);


};

extern QUEUE *newQUEUE(void (*d)(void *,FILE *),void (*f)(void *)){
	QUEUE *queue;
	queue = malloc(sizeof(QUEUE));
	assert(queue != 0);
	queue->display = d;
	queue->free = f;
	queue->list = newSLL(d,f);
	return queue;
}

extern void enqueue(QUEUE *items,void *value){
	insertSLL(items->list, sizeQUEUE(items), value);
	return;
}

extern void *dequeue(QUEUE *items){
	assert(sizeQUEUE(items) > 0);
	return removeSLL(items->list, 0);
}

extern void *peekQUEUE(QUEUE *items){
	assert(sizeQUEUE(items) > 0);
	return getSLL(items->list, 0);
}

extern int sizeQUEUE(QUEUE *items){
	return sizeSLL(items->list);
}

extern void displayQUEUE(QUEUE *items,FILE *fp){
	if(sizeQUEUE(items)==0){
		fprintf(fp, "<>");
		return;
	}

	int i = 0;
	fprintf(fp,"<");
	for(i = 0; i < sizeQUEUE(items); i++){
		if(i != 0){
			fprintf(fp, ",");
		}
		items->display(getSLL(items->list, i),fp);
	}
	fprintf(fp, ">");
	return;
}


extern void displayQUEUEdebug(QUEUE *items,FILE *fp){
	displaySLLdebug(items->list,fp);
	return;
}

extern void freeQUEUE(QUEUE *items){
	freeSLL(items->list);
	free(items);
}
