#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

extern int *bitmap;


int allocate_map(void);
int allocate_pid(void);
void release_pid(int pid);