#ifndef _CPU_H
#define _CPU_H

#include<stdint.h>

#if __riscv_xlen == 64
typedef int64_t xlen_t;
typedef int64_t reg_t;
typedef uint64_t ureg_t;

#else 
typedef int32_t xlen_t;
typedef int32_t reg_t;
typedef uint32_t ureg_t;

#endif
 
typedef uint32_t ilen_t;

#define enable_global_int() asm("csrsi mstatus,0x8")
#define disable_global_int() asm("csrci mstatus,0x8")
#define enable_time_int() asm("li t0,0x80\n\tcsrs mie,t0")
#define disable_time_int() asm("li t0,0x80\n\tcsrc mie,t0")


extern void cpu_sr_reset(xlen_t);
extern xlen_t   cpu_sr_set(void);

extern void set_mstatus(reg_t);
extern reg_t get_mstatus();
extern reg_t get_mstatus_and_disable_int(); 

#define atomic_begin(_sr)  ({_sr=get_mstatus_and_disable_int();})
#define atomic_end(_sr)   set_mstatus(_sr)

#endif

