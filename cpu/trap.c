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

void ecall_handler(gprs_t *gpr)
{
	extern long do_syscall();
	gpr->a0=do_syscall(gpr->a0,gpr->a1,gpr->a2,gpr->a3,gpr->a4,gpr->a5);
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

static void interrupt(reg_t mcause,frame_t *regs)
{
	switch(mcause&0xff)
	{
		case IRQ_S_SOFT   :
		case IRQ_H_SOFT   :
		case IRQ_M_SOFT   :
		case IRQ_S_TIMER  :
		case IRQ_H_TIMER  :
		case IRQ_M_TIMER  :
		case IRQ_S_EXT    :
		case IRQ_H_EXT    :
		case IRQ_M_EXT    :
		case IRQ_COP      :
		case IRQ_HOST     :
		default           : exit(mcause<<4);
	}
}

static void exception(reg_t mcause,frame_t *regs)
{
	switch(mcause)
	{
		case  CAUSE_MISALIGNED_FETCH :
		case  CAUSE_FETCH_ACCESS :
		case  CAUSE_ILLEGAL_INSTRUCTION :
		case  CAUSE_BREAKPOINT :
		case  CAUSE_MISALIGNED_LOAD :
		case  CAUSE_LOAD_ACCESS :
		case  CAUSE_MISALIGNED_STORE :
		case  CAUSE_STORE_ACCESS :exit(mcause<<4);
		case  CAUSE_USER_ECALL :
		case  CAUSE_SUPERVISOR_ECALL :
		case  CAUSE_HYPERVISOR_ECALL :
		case  CAUSE_MACHINE_ECALL : write_csr(mepc,read_csr(mepc)+4);break;
		case  CAUSE_FETCH_PAGE_FAULT :
		case  CAUSE_LOAD_PAGE_FAULT :
		case  CAUSE_STORE_PAGE_FAULT :
		default : exit(mcause<<4);
	}
}

reg_t mtrap(reg_t mcause,frame_t *regs)
{
	if(mcause < 0)
		interrupt(mcause,regs);
	else 
		exception(mcause,regs);
}

void halt(xlen_t mcause,xlen_t mepc)
{
	extern void exit(int);
	disable_global_int();
	printf("halt ,mcause=%lx ,mepc=%lx\n",mcause,mepc);
	exit(0);
}



