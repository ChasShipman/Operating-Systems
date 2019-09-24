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
 *an abstract singly-linked
 *data structure.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "sll.h"


typedef struct node{
	void *value;
	struct node *next;
} NODE;

struct sll{
	void (*display)(void *, FILE *);
	void (*free)(void *);
	NODE *head;
	NODE *tail;
	int size;
};

static NODE *newNODE(){
	NODE *node = malloc(sizeof(NODE));
	node->value = 0;
	node->next = 0;
	return node;
}

extern SLL *newSLL(void (*d)(void *,FILE *),void (*f)(void *)){
	//d is display function;
	//f is free fucntion;

	SLL *items = malloc(sizeof(SLL));
	assert(items != 0);
	items->head = 0;
	items->tail = 0;
	items->size = 0;
	items->display = d;
	items->free = f;
	return items;
}


extern void insertSLL(SLL *items,int index,void *value){
	assert(index >= 0 && index <= items->size);
	NODE *newNode= newNODE();
	newNode->value = value;

	if(items->size == 0){
		items->head = items->tail = newNode;
		newNode->next = 0;
	}//if list is empty

	else if(index == 0){
		newNode->next = items->head;
		items->head = newNode;
	}//if adding to the front

	else if(index == items->size){
		items->tail->next = newNode;
		newNode->next = 0;
		items->tail = newNode;
	}//if adding to the end;

	else{ //if adding in the middle
		int i = 0;//for index in list
		NODE *prevTemp = items->head;//start at head

		while(i < index-1){//loop until on node before insertion
			prevTemp = prevTemp->next;
			i++;
		}
		NODE *afterTemp = prevTemp->next; //puts temp node on insertion.
		newNode->next = afterTemp;//newNode points to insertion
		prevTemp->next = newNode;//previous node points to new node, which points to...do I need to update where newNode->next points to? TEST!
	}
	items->size++;
	return;
}



extern void *removeSLL(SLL *items,int index){
	assert(items->size > 0 && index >= 0 && index < items->size);
	void *valueHolder = 0;


	if(items->size == 1){
		valueHolder= items->head->value;
		NODE *tempNode = items->head;
		items->head = items->head->next;
		free(tempNode);
	}

	else if(index == 0){
		valueHolder= items->head->value;
		NODE *tempHead = items->head;
		items->head = items->head->next;
		free(tempHead);
	}//if removing from the front

	else if(index == (items->size-1)){
		valueHolder= items->tail->value;
		NODE *prevTemp = items->head;//start at head
		NODE *tempTail = items->head->next;
		while(tempTail->next != 0 ){//loop until on last node
			prevTemp = tempTail;//move previous Node to node before tempTail
			tempTail = tempTail->next;//advance temptail;
		}
		items->tail = prevTemp;//repoint tail to previous Node
		items->tail->next = 0;//repoint node before tail to 0
		free(tempTail);
	}//if removing from the end;

	else{ //if removing from the middle
		int i = 0;//for index in list
		NODE *prevTemp = items->head;//start at head
		NODE *afterTemp = items->head;//start at head
		while(i < index){//loop until on node before deletion;
			prevTemp = afterTemp;//move previous node to node before deletion
			afterTemp = afterTemp->next;//move afterTemp to node to be deleted
			i++;
		}
		prevTemp->next = afterTemp->next; //repoints previous node to node after the noede to be deleted
		valueHolder= afterTemp->value;
		free(afterTemp);
	}
	items->size--;
	return valueHolder;

}

extern void unionSLL(SLL *recipient,SLL *donor){
	if(recipient->size == 0 && donor->size == 0){//if both lists are empty

		recipient->head = recipient->tail = donor->head = donor->tail= 0;
		return;
	}

	else if(recipient->size == 0 && donor->size != 0){//if recipient list is empty and donor is not

		recipient->size += donor->size;
		recipient->head = donor->head;
		recipient->tail = donor->tail;
		donor->head = donor->tail = 0;
		donor->size = 0;
	}

	else if(recipient->size != 0 && donor->size == 0){//if the donor is empty
		return;
	}

	else{//if both have stuff in them 
		recipient->size += donor->size;
		recipient->tail->next = donor->head;
		recipient->tail = donor->tail;
		donor->head = donor->tail = 0;
		donor->size = 0;

	}

	return;

}


extern void *getSLL(SLL *items,int index){
	assert(index >= 0 && index < items->size);
	void *valueHolder = 0;
	if(items->head == 0 && items->tail == 0 && items->size == 0){
		return valueHolder;
	}//if list is empty

	else if(index == 0){
		return items->head->value;
	}//if getting first value;

	else if(index == (items->size -1)){
		return items->tail->value;
	}//if getting last value

	else{ //if getting value from middle
		int i = 0;//for index in list
		NODE *tempNode = items->head;//start at head

		while(i != index){//loop until on index
			tempNode = tempNode->next;
			i++;
		}
		return tempNode->value;
	}
}

extern void *setSLL(SLL *items,int index,void *value){
	assert(index >= 0 && index <= items->size);
	void *valueHolder = 0;
	
	if(items->head == 0 && items->tail == 0 && items->size == 0){
		return valueHolder;
			}//if list is empty

	else if(index == 0){
		valueHolder = items->head->value;
		items->head->value = value;
		return valueHolder;
	}//if setting first value;

	else if(index == items->size){
		insertSLL(items, items->size, value);
		return items->tail->next;
	}//if getting last value

	else if(index == items->size -1){
		valueHolder = items->tail->value;
		items->tail->value = value;
		return valueHolder;
	}

	else{ //if getting value from middle
		int i = 0;//for index in list
		NODE *tempNode = items->head->next;//start at head

		while(i < index - 1){//loop until on index
			tempNode = tempNode->next;
			i++;
		}
		valueHolder = tempNode->value;
		tempNode->value = value;
		return valueHolder;
	}
}

extern int sizeSLL(SLL *items){
	return items->size;
}


extern void displaySLL(SLL *items,FILE *fp){
	NODE *tempNode = items->head;

	if (items->size == 0){

		fprintf(fp,"{}");
		return;
	}
	else {


		fprintf(fp, "{");
		while (tempNode->next != 0){

			items->display(tempNode->value, fp);
			fprintf(fp,",");
			tempNode = tempNode->next;
		}

		items->display(tempNode->value, fp);
		fprintf(fp,"}");

		return;
	}
}

extern void displaySLLdebug(SLL *items,FILE *fp){
	NODE *tempHead = items->head;
	NODE *tempTail = items->tail;

	if (items->size == 0){

		fprintf(fp,"head->{},tail->{}");
		return;
	}

	else{

		fprintf(fp, "head->{");
		while(tempHead->next != 0){
			items->display(tempHead->value, fp);
			fprintf(fp,",");
			tempHead = tempHead->next;
		}
		items->display(tempHead->value, fp);
		fprintf(fp, "},");
		fprintf(fp, "tail->{");
		items->display(tempTail->value,fp);
		fprintf(fp, "}");
		return;
	}
	return;
}


extern void freeSLL(SLL *items){
	if (items == 0 || items->head == 0 || items->tail == 0){
		free(items);
		return;
	}
	else{


		while (items->head){
			NODE *freeNode = items->head;
			freeNode->value = items->head->value;
			items->head = items->head->next;
			if(items->free != 0)
				items->free(freeNode->value);
			free(freeNode);
		}
	}
	free(items);
	return;
}

