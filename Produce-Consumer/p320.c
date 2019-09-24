#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "p320.h"

#define MIN_PID 300 //Min PID
#define MAX_PID 5000 //Max PID
#define bitmap_LENGTH 147 //(MAX_PID - MIN_PID)/32 because intergers are 32 bits

static int isBitSet(int *bitmap, int kth); //checks if bit has been set already
static void setBit(int *bitmap, int kth); //sets bit
static void clearBit(int *bitmap, int kth); //clears bit

int *bitmap;

int allocate_map(void) {	//creates bitmap
	bitmap = malloc(sizeof(int) * bitmap_LENGTH);
	if(bitmap){
		return 1;
	}
	else{
		return 0;
	}
}

int allocate_pid(void) {	//allocates and returns a pid
	int k = 0;
	while(k <= MAX_PID - MIN_PID) {
		if(isBitSet(bitmap, k)) {	//check if bit has been set
			k++;
		}
		else {
			setBit(bitmap, k); //sets bit
			return k + MIN_PID;
		}
	}
	return -1;
}

void release_pid(int pid) {	//releases a pid
	if(pid >= MIN_PID && pid <= MAX_PID) {
		int k = pid - MIN_PID; //convert pid to bit location
        clearBit(bitmap, k);//clear bit
	}
	else {
		printf("PID Out Of Range: 300 - 5000\n");
	}
}

static int isBitSet(int *bitmap, int kth){
    int bitSet = 1; //bit flag set to 1
    int i = kth/32;
    int kthPosition = kth % 32; //bit position of k
    
    bitSet = bitSet << kthPosition; //shift 1 k positions
    
    if(bitmap[i] & bitSet){ //check if bit is 1
        return 1;
    }
    else{
        return 0;
    }
}

static void setBit(int *bitmap, int kth){
    int bitSet = 1; //bit flag set to 1
    int i = kth/32;
    int kthPosition = kth % 32; //bit position of k
    bitSet = bitSet << kthPosition; //shift 1 k positions
    bitmap[i] = bitmap[i] | bitSet; // set bit
    return;
}

static void clearBit(int *bitmap, int kth){
    int bitSet = 1; //bit flag set to 1
    int i = kth/32;
    int kthPosition = kth % 32; //bit position of k
    
    bitSet = bitSet << kthPosition; //shift 1 k positions
    bitSet = ~bitSet; //complement of bit flag
    
    bitmap[i] = bitmap[i] & bitSet; //reset bit
    return;
}
