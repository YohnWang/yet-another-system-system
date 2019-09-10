#ifndef _TRAP_H
#define _TRAP_H

#include<cpu.h>


#ifndef NULL
#define NULL ((void*)0)
#endif


struct frame_t
{
	reg_t epc;   // 
    reg_t ra;    // Return address
    reg_t sp;    // Stack pointer
    reg_t gp;    // Global pointer
    reg_t tp;    // Thread pointer
    reg_t t0;    // Temporary
    reg_t t1;    // Temporary
    reg_t t2;    // Temporary
    reg_t s0;    // Saved register/frame pointer
    reg_t s1;    // Saved register
    reg_t a0;    // Function argument/return value
    reg_t a1;    // Function argument/return value
    reg_t a2;    // Function argument
    reg_t a3;    // Function argument
    reg_t a4;    // Function argument
    reg_t a5;    // Function argument
    reg_t a6;    // Function argument
    reg_t a7;    // Function argument
    reg_t s2;    // Saved register
    reg_t s3;    // Saved register
    reg_t s4;    // Saved register
    reg_t s5;    // Saved register
    reg_t s6;    // Saved register
    reg_t s7;    // Saved register
    reg_t s8;    // Saved register
    reg_t s9;    // Saved register
    reg_t s10;   // Saved register
    reg_t s11;   // Saved register
    reg_t t3;    // Temporary
    reg_t t4;    // Temporary
    reg_t t5;    // Temporary
    reg_t t6;    // Temporary
};
typedef struct frame_t frame_t;

extern void halt(xlen_t,xlen_t);
extern void syscall_handler(xlen_t sp[]);
extern xlen_t trap_handler(xlen_t mcause,xlen_t mepc,xlen_t sp[]);

enum syscall_num_t {SYS_EXIT=0,SYS_HALT,SYS_TASKSW,};



#endif
