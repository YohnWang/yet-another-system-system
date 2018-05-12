#ifndef _CPU_H
#define _CPU_H

#include<trap.h>

#define enable_global_int() asm("csrsi mstatus,0x8")
#define disable_global_int() asm("csrci mstatus,0x8")
#define enable_time_int() asm("li t0,0x80\n\tcsrs mie,t0")
#define disable_time_int() asm("li t0,0x80\n\tcsrc mie,t0")


extern void cpu_sr_reset(xlen_t);
extern xlen_t   cpu_sr_set(void);

#define atomic_begin() xlen_t _sr=cpu_sr_set()
#define atomic_end()   cpu_sr_reset(_sr)

#endif

