#ifndef SYSCALL_H
#define SYSCALL_H


#define SYS_restart		0
#define SYS_exit		1
#define SYS_task_create	2
#define SYS_task_switch 3
#define SYS_task_exit   4
#define SYS_int_disable 5
#define SYS_int_enable  6
#define SYS_time        7
#define SYS_

extern long syscall(long sysnum,long arg0,long arg1,long arg2,long arg3,long arg4);

#endif
