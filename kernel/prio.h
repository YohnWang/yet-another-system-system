#ifndef _PRIO_H
#define _PRIO_H

#include<task.h>

struct prio
{
	uint64_t prio_tab;
	tid_t    tid_map[sizeof(uint64_t)];
};
typedef struct prio prio;


int prio_add(prio *sche,tid_t id,int pr);
int prio_del(prio *sche,int pr);
int prio_get(prio *sche);
tid_t prio_tid(prio *sche);


#endif
