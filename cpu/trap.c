#include<encoding.h>
#include<trap.h>
#include<init.h>
#include<host.h>
#include<rrsched.h>
#include<task.h>
#include<ds.h>

extern void* __task_switch(struct trapframe sp[]);

void* syscall_handler(struct trapframe sp[])
{
	switch(sp->a0)
	{
		case SYS_EXIT:
			exit(sp->a1);
			break;
		case SYS_HALT:
			
			break;
		case SYS_TASKSW:
			return __task_switch(sp);
			break;
		default:
			printf("system call error,no such sysnum\n");
			exit(1);
	}
	return sp;
}

void* time_handler(struct trapframe sp[])
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
		return __task_switch(sp);
	}
}

void* trap(intptr_t mcause,intptr_t mepc,struct trapframe sp[])
{
	if(mcause<0)
	{
		sp->epc=mepc;//restore mepc
		//clear sign bit
		mcause<<=1;
		mcause>>=1;
		switch(mcause)
		{
			extern void next_timecmp(void);
			case 7://timer interrupt
				
				next_timecmp();
				return time_handler(sp);
				break;
			default:
				halt();
				break;
		}
	}
	else 
	{
		switch(mcause)
		{
			case 11://scall M-mode
				sp->epc=mepc+4;
				return syscall_handler(sp);
				break;
			default:
				halt();
				break;
		}
	}
	
	return sp;
}

void halt()
{
	extern void exit(int);
	disable_global_int();
	printf("halt ,mcause=%lx ,mepc=%lx ,mtval=%lx\n",read_csr(mcause),read_csr(mepc),read_csr(mbadaddr));
	exit(0);
}



