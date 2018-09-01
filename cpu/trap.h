#ifndef _TRAP_H
#define _TRAP_H

#include<cpu.h>
#include<stdint.h>


#ifndef NULL
#define NULL ((void*)0)
#endif



struct trapframe
{
    intptr_t ra;    // Return address
    intptr_t sp;    // Stack pointer
    intptr_t gp;    // Global pointer
    intptr_t tp;    // Thread pointer
    intptr_t t0;    // Temporary
    intptr_t t1;    // Temporary
    intptr_t t2;    // Temporary
    intptr_t s0;    // Saved register/frame pointer
    intptr_t s1;    // Saved register
    intptr_t a0;    // Function argument/return value
    intptr_t a1;    // Function argument/return value
    intptr_t a2;    // Function argument
    intptr_t a3;    // Function argument
    intptr_t a4;    // Function argument
    intptr_t a5;    // Function argument
    intptr_t a6;    // Function argument
    intptr_t a7;    // Function argument
    intptr_t s2;    // Saved register
    intptr_t s3;    // Saved register
    intptr_t s4;    // Saved register
    intptr_t s5;    // Saved register
    intptr_t s6;    // Saved register
    intptr_t s7;    // Saved register
    intptr_t s8;    // Saved register
    intptr_t s9;    // Saved register
    intptr_t s10;   // Saved register
    intptr_t s11;   // Saved register
    intptr_t t3;    // Temporary
    intptr_t t4;    // Temporary
    intptr_t t5;    // Temporary
    intptr_t t6;    // Temporary
    intptr_t epc;
};




extern void halt();
extern void* syscall_handler(struct trapframe *sp);
extern xlen_t trap_handler(xlen_t mcause,xlen_t mepc,struct trapframe *sp);

enum syscall_num_t {SYS_EXIT=0,SYS_HALT,SYS_TASKSW,};



#endif
