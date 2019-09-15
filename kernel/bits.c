#include<stdint.h>

int ntz(uint64_t x)
{
	if(x==0)
	    return 64;
	int n=1;
	if((x&0x00000000FFFFFFFF)==0) {n=n+32;x=x>>32;}
	if((x&0x000000000000FFFF)==0) {n=n+16;x=x>>16;}
	if((x&0x00000000000000FF)==0) {n=n+8;x=x>>8;}
	if((x&0x000000000000000F)==0) {n=n+4;x=x>>4;}
	if((x&0x0000000000000003)==0) {n=n+2;x=x>>2;}
	return n-(x&1);
}

int bit_get(uint64_t pt,int x)
{
	if(pt&(1LLU<<x))
		return 1;
	return 0;
}

uint64_t bit_set(uint64_t pt,int x)
{
	return (pt)|(1LLU<<x);
}

uint64_t bit_clr(uint64_t pt,int x)
{
	return pt&~(1LLU<<x);
}
