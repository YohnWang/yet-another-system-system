#include<encoding.h>
#include<trap.h>
#include<init.h>
#include<host.h>
#include<rrsched.h>
#include<task.h>
#include<ds.h>

extern void task_switch(xlen_t sp[]);

void syscall_handler(xlen_t sp[])
{
#define sysarg(offset) (sp[9+(offset)])

	switch(sysarg(0))
	{
		case SYS_EXIT:
			exit(sysarg(1));
			break;
		case SYS_HALT:
			
			break;
		case SYS_TASKSW:
			task_switch(sp);
			break;
		default:
			printf("system call error,no such sysnum\n");
			exit(1);
	}
#undef sysarg
}

void time_handler(xlen_t sp[])
{
	//
	extern struct prio sched;
	extern struct heap slp_q;
	//printf("time int sp=%lx\n",sp);
	if(!heap_empty(&slp_q))
	{
		tid_t id=slp_q.a[0];
		if(get_finish_time(id)<get_time())
		{
			int pr=get_tprio(id);
			prio_add(&sched,id,pr);
			heap_pop(&slp_q);
			//task_awake(id);
			//printf("pr=%d resume\n",pr);
		}
	}
	int pr=prio_get(&sched);
	if(pr>=0)
	{
		task_next_task(prio_tid(&sched));
		task_switch(sp);
	}
}


xlen_t trap_handler(xlen_t mcause,xlen_t mepc,xlen_t sp[])
{

	if(mcause<0)
	{
		//clear xlen-1 bit
		mcause<<=1;
		mcause>>=1;
		//mcause=mcause&(((ureg_t)~0)>>1);
		switch(mcause)
		{
			extern void next_timecmp(void);
			case 7://timer interrupt
				
				next_timecmp();
				sp[31]=mepc;
				time_handler(sp);
				break;
			default:
				halt(mcause,mepc);
				break;
		}
	}
	else 
	{
		switch(mcause)
		{
			case 11://scall M-mode
				sp[31]=mepc+4;
				syscall_handler(sp);
				mepc+=4;
				break;
			default:
				halt(mcause,mepc);
				break;
		}
	}
	return mepc;
}

void halt(xlen_t mcause,xlen_t mepc)
{
	extern void exit(int);
	disable_global_int();
	printf("halt ,mcause=%lx ,mepc=%lx\n",mcause,mepc);
	exit(0);
}



