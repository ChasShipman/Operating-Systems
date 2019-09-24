#ifndef __PROCESSSTRUCT_INCLUDED__
#define __PROCESSSTRUCT_INCLUDED__

#include <stdio.h>

typedef struct processStruct PS; //struct to hold process info

extern PS *newProcessStruct(int timeArrive, int priority, int processorTime); //constructor

extern pid_t getPID(PS *process); //get process id of process

extern int getTicks(PS *process); //get count of ticks of process
extern int getArrival(PS *process); //get the arrival time of the process
extern int getPriority(PS *process); //get the priority of the process
extern int getBurstTime(PS *process); //get the burst time (processor time) of the process

extern void setPID(PS *process, pid_t holderPID); //set the process id of the process
extern void incrTicks(PS *process); //increment the ticks of process
extern void decrPriority(PS *process); //decrement the priority of the process

//----------functions we were told to have-----------//
extern void terminateProcess(pid_t processPSD);
extern void suspendProcess(pid_t processPSD);
extern void startProcess(void);
extern void restartProcess(pid_t processPSD);

#endif
