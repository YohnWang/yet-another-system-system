#ifndef _TASK_H
#define _TASK_H

#include<stdint.h>
#include<cpu.h>


#define TASK_NUM (64)

enum Task_Status_t{NEW,RDY,RUN,BLK,EXT};

typedef intptr_t tid_t;
typedef struct tcb_t
{
	xlen_t *sp;
	int    status;
	uint64_t finish_time;
	char *task_name;
	
}tcb_t;


typedef struct task_attr_t
{
	xlen_t *sp;
	void *exit_handler;
	char *task_name;
	
}task_attr_t;

#endif
