#include<cpu.h>
#include<sem.h>

void wait(sem *s)
{
	xlen_t sr=cpu_sr_set();
	while(*s<=0)
		;
	(*s)--;
	cpu_sr_reset(sr);
}

void signal(sem *s)
{
	xlen_t sr=cpu_sr_set();
	(*s)++;
	cpu_sr_reset(sr);
}
