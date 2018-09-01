#include"define.h"

    .global trap_entry
    .text
    .align 2
trap_entry:
    STORE_CTX_REG
    
    csrr a0,mcause
    csrr a1,mepc
    mv   a2,sp
    .extern trap 
    jal  trap
    
    mv   sp,a0
    
    LOAD a0,31*REGBYTES(sp)
    csrw mepc,a0
    LOAD_CTX_REG
    mret
