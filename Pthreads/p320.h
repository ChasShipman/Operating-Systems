#ifndef __P320_INCLUDED__
#define __P320_INCLUDED__

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

extern int *bitmap;
extern int allocate_map(void);
extern int allocate_pid(void);
extern void release_pid(int pid);

#endif
