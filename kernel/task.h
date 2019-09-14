#ifndef _TASK_H
#define _TASK_H

#include<stdint.h>
#include<cpu.h>
#include<time.h>


#define TASK_NUM (64)

enum Task_Status_t{NEW,RDY,RUN,BLK,EXT};

typedef intptr_t tid_t;
typedef struct tcb_t
{
	reg_t *sp;
	reg_t pc;
	tid_t  mgr;
	int   prio;
	int    status;
	uint64_t finish_time;
	time_t excutable_time;
	char *task_name;
	
}tcb_t;


typedef struct task_attr_t
{
	xlen_t *sp;
	int   prio;
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
uint64_t get_finish_time(tid_t id);

void task_block();
void task_awake(tid_t id);
void task_set_status(tid_t id,int status);
int task_get_status(tid_t id);


#endif
