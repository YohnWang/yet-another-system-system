#ifndef _DS_H
#define _DS_H

#include<task.h>

typedef tid_t Etype;
#define HEAP_SIZE TASK_NUM

struct heap
{
    Etype a[HEAP_SIZE];
    int size;
};


Etype heap_pop(struct heap *h);
void heap_push(struct heap *h,Etype x);
int heap_full(struct heap *h);
int heap_empty(struct heap *h);

#endif

