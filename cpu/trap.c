#include<encoding.h>
#include<trap.h>
#include<init.h>
#include<host.h>
#include<rrsched.h>
#include<task.h>

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
{/*
	static int run=0;
	extern tid_t t1,t2;
	if(run<10)
	{
		task_next_task(t1);
	}
	else if(run<12)
	{
		task_next_task(t2);
	}
	else 
	{
		task_next_task(0);
		run=0;
	}
	run++;
	task_switch(sp);*/
}


xlen_t trap_handler(xlen_t mcause,xlen_t mepc,xlen_t sp[])
{

	if(mcause<0)
	{
		//clear xlen-1 bit
		mcause<<=1;
		mcause>>=1;
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
	printf("halt ,mcause=%llx ,mepc=%llx\n",mcause,mepc);
	exit(0);
}



