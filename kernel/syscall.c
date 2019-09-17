#include<cpu.h>
#include<syscall.h>
#include<stdint.h>

typedef long (*syscall_t)(long,long,long,long,long,long,long);


long do_syscall(long a0,long a1,long a2,long a3,long a4,long a5,long n)
{
	extern long task_creat(),task_switch();
	const static void *syscall_table[]=
	{
		[SYS_restart]=0,
		[SYS_exit]=0,
		[SYS_task_create]=task_creat,
		[SYS_task_switch]=task_switch,
	};
	syscall_t f=syscall_table[n];
	return f(a0,a1,a2,a3,a4,a5,n);
}

long syscall(long sysnum,...)
{
	long r;
	asm volatile ("ecall\n\t mv %[r],a0":[r]"=r"(r));
	return r;
}
