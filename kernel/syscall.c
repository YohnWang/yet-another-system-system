#include<cpu.h>
#include<syscall.h>
#include<stdint.h>

typedef long (*syscall_t)(long,long,long,long,long,long);


long do_syscall(long a0,long a1,long a2,long a3,long a4,long n)
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
	return f(a0,a1,a2,a3,a4,n);
}

long syscall(long sysnum,long arg0,long arg1,long arg2,long arg3,long arg4)
{
	register long a0 asm("a0") = arg0;
	register long a1 asm("a1") = arg1;
	register long a2 asm("a2") = arg2;
	register long a3 asm("a3") = arg3;
	register long a4 asm("a4") = arg4;
	register long n  asm("a5") = sysnum;
	asm volatile ("ecall":"+r"(a0):"r"(a0),"r"(a1),"r"(a2),"r"(a3),"r"(a4),"r"(n));
	return a0;
}
