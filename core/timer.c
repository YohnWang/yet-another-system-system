#include<stdint.h>


struct timer
{
	uint64_t tick;
	uint64_t timecmp;
	void (*event)();
};


