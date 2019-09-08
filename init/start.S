/* */
#include"define.h"

	.text 
	.align 2
	.global _start
_start:
	//set MPP 
	li t6, 0x1800
    csrw mstatus, t6
	//set trap-vector base-address register
	.extern trap_vector
    la t0, trap_vector
    csrw mtvec, t0

    /* initialize stack pointer */
    li  sp,0x80100000
    
    .extern init
    j init

    .end
    
