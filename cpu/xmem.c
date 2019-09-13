#include<stdint.h>

#if __riscv_xlen == 64
#define mem_place(name,size) uintptr_t name[size>>3]
#elif __riscv_xlen == 32
#define mem_place(name,size) uintptr_t name[size>>2]
#else 
#error
#endif


mem_place(MAIN_STACK,65536);
mem_place(TRAP_STACK,65536);
