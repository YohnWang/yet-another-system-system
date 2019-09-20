#include"define.h"
	.text
	.global get_mstatus
	.align 2
get_mstatus:
	csrr a0,mstatus
	ret 
	
	.global set_mstatus
set_mstatus:
	csrw mstatus,a0
	ret 
	
	.global get_mstatus_and_disable_int
get_mstatus_and_disable_int:
	csrr a0,mstatus
	csrci mstatus,0x8
	ret 
	
	
    
    
    .global trap_vector
    .text
    .align 2
trap_vector:
	
    STORE_CTX_REG
    csrrw zero,mscratch,sp

    //call xlen_t trap_handler(mcause,mepc,sp);
    csrr a0,mcause
    csrr a1,mepc
    mv a2,sp
    .extern trap_handler
    jal trap_handler
    csrw mepc,a0
    
    csrr sp,mscratch
    LOAD_CTX_REG
    
    mret


	.global task_switch
task_switch:
	//restore sp when trap occur ,it's important
	mv sp,a0
	
	//then store sp
	//first get the task id,and get this task's tcb:sp
	//get_tcb_sp(get_tid())
	jal get_tid
	jal get_tcb_sp
	//store sp
	STORE sp,0(a0)

	//switch to new task's tcb
	//first get the new task id,and get its sp
	jal get_next_tid
	mv t0,a0
	jal get_tcb_sp
	//get new sp
	LOAD sp,0(a0)

	//set task id as new task id
	mv a0,t0
	jal set_tid
	
	//restore context
	LOAD t0,31*REGBYTES(sp)
	csrw mepc,t0
	
	LOAD_CTX_REG

	mret


	.global system_call
	//void system_call(long sysnum,long arg1,long arg2,long arg3)
system_call:
	scall
	ret 
	
