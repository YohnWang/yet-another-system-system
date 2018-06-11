#include<stdint.h>
#include<time.h>
#include<cpu.h>

#define time_addr     (0x2000000+0xbff8)
#define timecmp_addr  (0x2000000+0x4000)

#define time (*(volatile uint64_t*)time_addr)
#define timecmp (*(volatile uint64_t*)timecmp_addr)


uint64_t get_time(void)
{
	return time;
}

void add_timecmp(uint64_t tick)
{
	xlen_t sr;
	atomic_begin(sr);
	timecmp=time+tick;
	atomic_end(sr);
}

void next_timecmp(void)
{
	add_timecmp(Time_Tick_Clk);
}

void delay(uint64_t t)
{
	time_t s=get_time();
	while(get_time()-s<t){}
}
