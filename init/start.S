/* */
#include"define.h"

	.global _mstart
	.text 
	.align 4
_mstart:
	//set MPP 
	li t6, 0x1800
    csrw mstatus, t6
	//set trap-vector base-address register
    //la t0, trap_vector
    la t0,trap_entry
    csrw mtvec, t0

    //initialize all general register
    li	x1, 0
    li	x2, 0
    li	x3, 0
    li	x4, 0
    li	x5, 0
	li	x6, 0
    li	x7, 0
    li	x8, 0
    li	x9, 0
    li	x10, 0
    li	x11, 0
    li	x12, 0
    li	x13, 0
    li	x14, 0
    li	x15, 0
    li	x16, 0
    li	x17, 0
    li	x18, 0
    li	x19, 0
    li	x20, 0
    li	x21, 0
    li	x22, 0
    li	x23, 0
    li	x24, 0
    li	x25, 0
    li	x26, 0
    li	x27, 0
    li	x28, 0
    li	x29, 0
    li	x30, 0
    li	x31, 0

    /* initialize global pointer */
    la	gp, _gp
    la  sp,_stack
    
    .extern init
    j init
     
	
	.global cpu_sr_set
	.align 4
cpu_sr_set:
	csrr a0,mstatus
	csrci mstatus,0x8
	ret 
	
	.global cpu_sr_reset
cpu_sr_reset:
	csrw mstatus,a0
	ret 
	
	.global system_call
	//void system_call(long sysnum,long arg1,long arg2,long arg3)
system_call:
	scall
	ret 
    .end
    
