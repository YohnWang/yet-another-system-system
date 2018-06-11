#ifndef _SEM_H
#define _SEM_H

#include<task.h>

struct sem
{
	long counter;
	long wait_num;
	tid_t wait_arr[TASK_NUM];
	
};

typedef struct sem sem;
void sem_wait(sem *s);
void sem_signal(sem *s);


#endif
