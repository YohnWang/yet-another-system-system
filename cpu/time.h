#ifndef TIME_H
#define TIME_H

#include<stdint.h>


#define Time_Tick_Clk (500)

typedef uint64_t time_t;

extern uint64_t get_time(void);
extern void add_timecmp(uint64_t );
extern void next_timecmp(void);


#endif 

