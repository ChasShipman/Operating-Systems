#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include "sll.h"
#include "string.h"

#define MAX_LINE 80 /* The maximum length command */

int readCommands(char *commandArray){ //reads commands into a char *array

		fgets(commandArray, MAX_LINE, stdin);
		strtok(commandArray, "\n");
		
		if(strcmp(commandArray, "exit") == 0){//if exit return 0 should_run flag
			return 0;//stop reading
		}
		if(strcmp(commandArray, "history") == 0)//return 2 for history flag
			return 2;
		
	return 1;//return one otherwise to continue;
}

void tokenizeCommands(char *tempCommands, char *commandArray, char *args[]){//separate command by space
	strcpy(tempCommands, commandArray);
	args[0] = strtok(tempCommands, " ");
	int commands = 0;
	while(args[commands] != NULL){
		commands++;
		args[commands] = strtok(NULL, " ");
	}
	return;
}

int doesWait(char *args[]){//used for '&' waitFlag
	int commands = 0;
	while(args[commands] != NULL)
		commands++;
	
	if(strcmp(args[commands-1], "&") == 0){
		args[commands - 1] = NULL;
		return 0;
	}

	return 1;
}

void printHistory(SLL *commandHistory){
	
	if(sizeSLL(commandHistory) == 0){
		printf("No commands in history.\n");
	}

	else{
		char *tempHistory = malloc(sizeof(char) * MAX_LINE);
		int i = 0;
		while(i <= sizeSLL(commandHistory)-1){
			tempHistory[i] = *getSTRING(getSLL(commandHistory, i));
			i++;
		}
		i = 0;
		while(i != 10 && i < sizeSLL(commandHistory)){
			printf("%d ", sizeSLL(commandHistory) -i);
	//		displaySLL(commandHistory, stdout);
//			printf("%s\n",tempHistory);
//			printf("%c\n",tempHistory[i]);
			displaySTRING(getSLL(commandHistory, sizeSLL(commandHistory) - i - 1), stdout);
			printf("\n");
			i++;
		}
	}
}


int main(){

	char *args[MAX_LINE/2 + 1]; /* command line arguments */
	int should_run = 1; /* flag to determine when to exit program */
	int waitFlag = 1;
	SLL *commandHistory = newSLL(displaySTRING, freeSTRING);

	while (should_run) { 
		char *commandArray = malloc(sizeof(char) * MAX_LINE);

		printf("osh>"); 
		fflush(stdout);

		/*
		 * After reading user input, the steps are:
		 * (1) fork a child process using fork()	
		 * (2) the child process will invoke execvp()
		 * (3) if command included &, parent will invoke wait() 
		 */
		
		int historyFlag = should_run = readCommands(commandArray);
		int addHistoryFlag = 1;
		if(historyFlag == 2){//printHistory
			addHistoryFlag = 0;
			printHistory(commandHistory);
		}
		
		else{//addToHistor
			if(commandArray[0] == '!'){
				addHistoryFlag = 0;
				if(commandArray[1] == '!'){//give last command
					if(sizeSLL(commandHistory) != 0){
						commandArray = getSTRING(getSLL(commandHistory, sizeSLL(commandHistory)-1));
						printf("%s\n", commandArray);
					}
					else{
						printf("No commands in history.\n");
					}
				}
				else{//give !N command
					int index = atoi(&commandArray[1]);
					if(index > sizeSLL(commandHistory)){
						printf("No such command in history.\n");
					}
					else{
						if(sizeSLL(commandHistory) > 10){//if more than 10 commands
							index = 11 + index;
							commandArray = getSTRING(getSLL(commandHistory, sizeSLL(commandHistory)-index));
						}
						else commandArray = getSTRING(getSLL(commandHistory, index-1));
					}
					printf("%s\n", commandArray);
				}

			} 
		
		}
		if(addHistoryFlag)
			insertSLL(commandHistory, sizeSLL(commandHistory), newSTRING(commandArray));
		addHistoryFlag = 1;	
		
		char *tempCommands = malloc(sizeof(char) * MAX_LINE);
		tokenizeCommands(tempCommands, commandArray, args);
		waitFlag = doesWait(args);
		
		
//-----------fork process-------------//
		pid_t pid;
		pid = fork(); //(1)

		if(pid < 0){ //(2) child process failed to be created
			printf("Fork failed.\n");
			return 0;

		}

		else if(pid == 0){//child process created
			execvp(args[0], args);
		}
		else{//parent waites for child process if '&' is used, figure out how to invoke this
			if(waitFlag){//use a flag!
//				printf("WAITING\n");
				wait(NULL);
			}
		}
		//printf("LOOK HERE: %s\n", commandArray);
//		printf("should_run = %d\n", should_run);
	}
	waitFlag = 1;
	should_run = 1;
	return 0;
}
