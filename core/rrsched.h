#ifndef _RRSCHED_H
#define _RRSCHED_H

#include<task.h>

void rr_add(tid_t id);
void rr_del(tid_t id);
tid_t rr_next(void);

#endif
