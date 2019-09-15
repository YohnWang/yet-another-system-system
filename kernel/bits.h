#ifndef BITS_H
#define BITS_H
#include<stdint.h>
extern int ntz(uint64_t x);
extern int bit_get(uint64_t pt,int x);
extern uint64_t bit_set(uint64_t pt,int x);
extern uint64_t bit_clr(uint64_t pt,int x);

#endif

