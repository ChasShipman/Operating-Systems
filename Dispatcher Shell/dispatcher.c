#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "scanner.h" //included to be able to call function to read tokens in file
#include "queue.h" //custom queue struct
#include "processStruct.h" //custom process struct to store process data

//---Below are declarations for functions designed to carry out dispatcher methods---//
void readFile(FILE *fp, QUEUE *queue, char *token);//reads file into a dispatcher queue.
void dispatchProcesses(QUEUE *queue, PS *process); //dispatcher queue used to dispatch processes to priority queues
PS *findNextProcess(PS *process);//find next process to run from priority queues
PS *forkProcess(PS *process); //fork a new process if it has not been started
PS *decrementProcessPriority(PS *process); //decrement process priority (if not system level) if suspended
PS *processPriorityCleanUp(PS *process); //helper to catch process priority decrements of remaining processes

//-----different priority queues-----//
QUEUE *dispatcher; //dispatcher that is read into
QUEUE *systemLevelPriority;
QUEUE *userHighPriority;
QUEUE *userMidPriority;
QUEUE *userLowPriority;

//different flags to monitor state of queues and processes
int isEmpty;//is queue empty
int burstChecker; //check tick time of process
int isRunning; //is process running

pid_t pid;//process id global variable

int main(int argc, char **argv) {
	
	if(argc < 2){
		printf("ERROR:\n\tCorrect Use: ./dispatcher fileName\n");
		exit(1);
	}
	//---initalize variables---//
	dispatcher = newQUEUE(NULL,NULL);
	systemLevelPriority = newQUEUE(NULL, NULL);
	userHighPriority = newQUEUE(NULL, NULL);
	userMidPriority = newQUEUE(NULL, NULL);
	userLowPriority = newQUEUE(NULL, NULL);

	isEmpty = 0;
	burstChecker = 0;
	isRunning = 0;
    
    //---open file and read into dispatcher queue--//
	FILE *fp = fopen(argv[1],"r");
	char *token = 0;
	readFile(fp, dispatcher, token);
	fclose(fp);
    //initalize two processInfo object pointers//
	PS *tempProcess = 0;
	PS *dispatchHelper = 0;

	while(sizeQUEUE(dispatcher) > 0){//while dispatcher queue not empty dispatch processes
		dispatchProcesses(dispatcher, dispatchHelper);
		while((sizeQUEUE(dispatcher) > 0 && burstChecker < getArrival(peekQUEUE(dispatcher)) ) || (sizeQUEUE(dispatcher) == 0 && isEmpty)){
            //while processes is running (or if not found)
            //find next process
			if(!isRunning){//if not running
				isRunning = 1; //flag running
				tempProcess = findNextProcess(tempProcess); //find next
				if(isRunning){ //if a process found, run it
					if(getTicks(tempProcess) == 0){ //if process not found, fork it.
						tempProcess = forkProcess(tempProcess);
					}
					else{	//process has already been started, so restart it
						restartProcess(getPID(tempProcess));
					}
					incrTicks(tempProcess);
					usleep(990000); //sleep for ~1 second. making it sleep for 1 second breaks process
					burstChecker++;
				}

			}

			else{	// process alread running
				if(getTicks(tempProcess) >= getBurstTime(tempProcess)){//if process finished, terminate it.
					terminateProcess(getPID(tempProcess));
					isRunning = 0; //process not running
				}
				else{
					if(getPriority(tempProcess) == 0){	//means system level process
						incrTicks(tempProcess);
						burstChecker++;
						usleep(990000); //sleep for ~1 second. making it sleep for 1 second breaks process
						isRunning = 1; //keep running system level process
					}
					else{	//not system level process
                        suspendProcess(getPID(tempProcess));
                        isRunning = 0; //system level is suspended after 1 tick
						tempProcess = decrementProcessPriority(tempProcess); //if process suspended and not done, decrement priority of process
					}
				}
			}
		}
        
		//check on processes that are still running
		if(isRunning && getBurstTime(tempProcess) > getTicks(tempProcess)){ //if process is still running
			if(getPriority(tempProcess) != 0){//ignore system level process
				tempProcess = processPriorityCleanUp(tempProcess); //fix priority of user level processess
			}
		}
		else if(isRunning){	//process still running
			terminateProcess(getPID(tempProcess)); //terminate, must have reached burst time
			isRunning = 0;//process not running anymore
		}

	}
}

void readFile(FILE *fp, QUEUE *dispatcher, char *token){//read file into dispater queue
    
	int arrival, priority, processorTime; //process info variable
	token = readToken(fp); //read first token
	if(token == NULL){
		printf("ERROR:\n\t File is EMPTY\n");
		exit(0);
	}
    //while loop to read in file
	while(token){
		token[strlen(token)-1] = 0;
		arrival = atoi(token);    //get arrival time

		token = readToken(fp);
		token[strlen(token)-1] = 0;
		priority = atoi(token);    //get priority

		token = readToken(fp);
		processorTime = atoi(token);    //get burst time (processor time)
		enqueue(dispatcher, newProcessStruct(arrival,priority,processorTime)); //enqueue proceses to dispatcher queue
		token = readToken(fp);
	}
}

void dispatchProcesses(QUEUE *dispatcher, PS *dispatchHelper){ //dispatch processes to appropriate queues
    while(sizeQUEUE(dispatcher) > 0 && burstChecker >= getArrival(peekQUEUE(dispatcher))){//while processes are arriving, dispatch them
		dispatchHelper = dequeue(dispatcher);
		if(getPriority(dispatchHelper) == 0) //if sys level priority
			enqueue(systemLevelPriority,dispatchHelper);
		else if(getPriority(dispatchHelper) == 1) //if high user level priority
			enqueue(userHighPriority,dispatchHelper);
		else if(getPriority(dispatchHelper) == 2) //if mid level user priority
			enqueue(userMidPriority, dispatchHelper);
		else if(getPriority(dispatchHelper) == 3) //if low level user priority
			enqueue(userLowPriority, dispatchHelper);
		else
			printf("ERROR: Invalid Process dispatch\n");

		isEmpty = 1; //queues not empty
	}
}

PS *findNextProcess(PS *tempProcess){ //find next process
    //find next highest priority process
	if(sizeQUEUE(systemLevelPriority) > 0){//sys level is highest priorty
		tempProcess = dequeue(systemLevelPriority);
	}
	else if(sizeQUEUE(userHighPriority) > 0){//user high priority
		tempProcess = dequeue(userHighPriority);
	}
	else if(sizeQUEUE(userMidPriority) > 0){//user mid priority
		tempProcess = dequeue(userMidPriority);
	}
	else if(sizeQUEUE(userLowPriority) > 0){//user low priority
		tempProcess = dequeue(userLowPriority);
	}
	else{
		isRunning = 0; //no processes running
		isEmpty = 0; //queues empty
	}
	return tempProcess;
}

PS *forkProcess(PS *tempProcess){//start new process
	pid = fork();
	if(pid == 0){
		startProcess(); //start child process
		exit(0);
	}
	else{
		setPID(tempProcess, pid); //return from parent process
		if(getTicks(tempProcess) >= getBurstTime(tempProcess)){ //if ticks == burst time (processor time)
			terminateProcess(pid); //terminate process
			isRunning = 0; //process not running
		}
	}
	return tempProcess;
}

PS *decrementProcessPriority(PS *tempProcess){//decrement process priority
	if(getPriority(tempProcess) == 1)//if high user priority
		enqueue(userMidPriority,tempProcess);//enqueue high priority process to mid priority queue so it runs first
	else if (getPriority(tempProcess) == 2)//if mid user priority
		enqueue(userLowPriority,tempProcess); //enqueue mid priority process to low priority queue so it runs first
	else if (getPriority(tempProcess) == 3)//if low user priority
		enqueue(userLowPriority,tempProcess);//enqueue low priority process to low priority queue so it runs first
	else
		printf("ERROR:\n\t Invalid Process decrprocpr\n");
	decrPriority(tempProcess); //decrement priority of process
	return tempProcess;
}

PS *processPriorityCleanUp(PS *tempProcess){//decrement process priority of still running processes
	suspendProcess(getPID(tempProcess)); //suspend non system level process
	if(getPriority(tempProcess) == 1)//if high user priority
		enqueue(userMidPriority,tempProcess);//enqueue high priority process to mid priority queue so it runs first
	else if (getPriority(tempProcess) == 2)//if mid user priority
		enqueue(userLowPriority,tempProcess);//enqueue mid priority process to low priority queue so it runs first
	else if (getPriority(tempProcess) == 3)//if low user priority
		enqueue(userLowPriority,tempProcess);//enqueue low priority process to low priority queue so it runs first
	else
		printf("ERROR:\n\t Invalid Process cleanup\n");
	decrPriority(tempProcess);//decrement priority of process
	isRunning = 0; //process not running
	return tempProcess;
}
