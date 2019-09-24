#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include "processStruct.h"


struct processStruct { //struct to hold process info

	pid_t pid;	//process id
	
    int countTicks;	//how long process has been running
	int timeArrive; //time process arrives
	int priority; //priority of process
	int burstTime; //burst time (processor time) of process
};

PS *newProcessStruct(int timeArrive, int priority, int burstTime) { //constructor for processStruct object
    PS *process = malloc(sizeof(PS));
	assert(process != 0);
	process->pid = 0;
	process->countTicks = 0;
	process->timeArrive = timeArrive;
	process->priority = priority;
	process->burstTime = burstTime;
	return process;
}

pid_t getPID(PS *process) { //get process id
	return process->pid;
}

int getTicks(PS *process) { //get count of ticks of process
	return process->countTicks;
}

int getArrival(PS *process) { //get arrival time of process
	return process->timeArrive;
}

int getPriority(PS *process) { //get priority of process
	return process->priority;
}

int getBurstTime(PS *process) { //get burst time (processor time) of project
	return process->burstTime;
}

void setPID(PS *process, pid_t holderPID) { //set the process id
	process->pid = holderPID;
    return;
}

void incrTicks(PS *process) { //increment the ticks counter
	process->countTicks = process->countTicks + 1;
    return;
}

void decrPriority(PS *process) { //decrement the priority
    if(process->priority == 3 || process->priority == 0) {
        return;
    }
    else {
        process->priority = process->priority + 1;
    }
    return;
}


//-------functions we were told to have-------//
void terminateProcess(pid_t processPID){ //terminate process
	kill(processPID, SIGINT);
	waitpid(processPID, NULL, WUNTRACED);
    return;
}

void suspendProcess(pid_t processPID){//suspend process
	kill(processPID, SIGTSTP);
	waitpid(processPID, NULL, WUNTRACED);
    return;
}

void startProcess(){//start process
	execvp("./process",NULL);
    return;
}

void restartProcess(pid_t processPID){//restart process
	kill(processPID, SIGCONT);
    return;
}
