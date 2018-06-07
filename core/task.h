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
	tid_t  mgr;
	int    status;
	uint64_t finish_time;
	char *task_name;
	
}tcb_t;


typedef struct task_attr_t
{
	xlen_t *sp;
	char *task_name;
	void *exit_handler;
	tid_t mgr;
	
}task_attr_t;



tid_t task_creat(void (*task)(),task_attr_t attr);
void task_next_task(tid_t id);
void task_sche(void);
void task_sleep(uint64_t t);

tid_t get_mgr(tid_t id);
tid_t get_tid();


#endif
