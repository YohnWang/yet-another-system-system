#ifndef _CPU_H
#define _CPU_H

#include<stdint.h>
#include<encoding.h>

typedef int64_t xlen_t;
typedef long reg_t;
typedef unsigned long ureg_t;

#define enable_global_int() asm("csrsi mstatus,0x8")
#define disable_global_int() asm("csrci mstatus,0x8")
#define enable_time_int() asm("li t0,0x80\n\tcsrs mie,t0")
#define disable_time_int() asm("li t0,0x80\n\tcsrc mie,t0")

#define read_mstatus_and_disable_int() ({ unsigned long _tmp; \
										asm volatile ( "csrr %0,mstatus \n\t \
														csrci mstatus,0x8 ":"=r"(_tmp) );\
											_tmp; })

typedef ureg_t atomic_t;
#define atomic_begin(_sr)  ({_sr=read_mstatus_and_disable_int();})
#define atomic_end(_sr)   write_csr(mstatus,_sr)

#define scall(snum,args) ({})

#endif

