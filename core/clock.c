#include<clock.h>


static volatile clk_t time_tick=0;


clk_t clock(void)
{
	return time_tick;
}

void _inc_tick_trap(void)
{
	time_tick++;
}
