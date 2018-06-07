#ifndef _ALLOC_H
#define _ALLOC_H

#ifndef NULL
#define NULL ((void*)0)
#endif

#include<stdint.h>

typedef uintptr_t size_t;
void* mm_malloc(size_t size);
void mm_free(void *bp);


#endif
