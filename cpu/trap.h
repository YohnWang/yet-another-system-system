#ifndef _TRAP_H
#define _TRAP_H

#include<stdint.h>
#include<stdarg.h>

#if __riscv_xlen == 64
typedef int64_t xlen_t;


#else 
typedef int32_t xlen_t;


#endif
 
typedef uint32_t ilen_t;



#ifndef NULL
#define NULL ((void*)0)
#endif
typedef typeof(sizeof(0)) size_t;
extern void halt(xlen_t,xlen_t);

extern int putchar(int ch);
extern void syscall_handler(xlen_t sp[]);
extern xlen_t trap_handler(xlen_t mcause,xlen_t mepc,xlen_t sp[]);
extern void printstr(const char* s);
extern void printhex(uint64_t x);


extern int printf(const char* fmt, ...);
extern int sprintf(char* str, const char* fmt, ...);
extern void sprintf_putch(int ch, void** data);
extern void* memcpy(void* dest, const void* src, size_t len);
extern void* memset(void* dest, int byte, size_t len);
extern size_t strlen(const char *s);
extern size_t strnlen(const char *s, size_t n);
extern int strcmp(const char* s1, const char* s2);
extern char* strcpy(char* dest, const char* src);
extern long atol(const char* str);

#endif
