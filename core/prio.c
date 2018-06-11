#include<prio.h>


static int ntz(uint64_t x)
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

static int bit_get(uint64_t pt,int x)
{
	if(pt&(1LLU<<x))
		return 1;
	return 0;
}

static uint64_t bit_set(uint64_t pt,int x)
{
	return (pt)|(1LLU<<x);
}

static uint64_t bit_clr(uint64_t pt,int x)
{
	return pt&~(1LLU<<x);
}

int prio_add(prio *sche,tid_t id,int pr)
{
	uint64_t pt=sche->prio_tab;
	if(bit_get(pt,pr))
		return -1;
	sche->prio_tab=bit_set(pt,pr);
	sche->tid_map[pr]=id;
	return 0;
}

int prio_del(prio *sche,int pr)
{
	uint64_t pt=sche->prio_tab;
	sche->prio_tab=bit_clr(pt,pr);
	return 0;
}

int prio_get(prio *sche)
{
	uint64_t pt=sche->prio_tab;
	int pr=ntz(pt);
	if(pr<sizeof(sche->prio_tab)*8)
		return pr;
	return -1;
}

tid_t prio_tid(prio *sche)
{
	int pr=prio_get(sche);
	if(pr<0)
		return -1;
	else 
		return sche->tid_map[pr];
}


