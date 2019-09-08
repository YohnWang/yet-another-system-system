#include<stdint.h>
#include<cpu.h>
#include<host.h>

static void init_bss(void)
{
	extern ureg_t __bss_start[],__bss_end[];
	for(int i=0;&__bss_start[i]<=__bss_end;i++ )
		__bss_start[i]=0;
}
/*
static void init_sbss(void)
{
	extern ureg_t __sbss_start[],__sbss_end[];
	for(int i=0;&__sbss_start[i]<=__sbss_end;i++ )
		__sbss_start[i]=0;
}
*/
#ifndef NULL
#define NULL ((void*)0)
#endif

void __attribute__((weak)) exit(int rvalue)
{
	extern int printf(const char*,...);
	disable_global_int();
	//printf("this exit function is default\n");
	//printf("exit!!!code: %d\n",rvalue);
	tohost=1|(rvalue<<1);
	for(;;){}
}

void init(void)
{
	extern int main(int,char*[]);
	extern struct prio sched;
	//init_sbss();
	init_bss();
	extern void init_heap(void);
	init_heap();
	char *argv[]={"yyos",NULL};
	prio_add(&sched,0,63);
	exit(main(1,argv));
}
