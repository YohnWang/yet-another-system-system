#ifndef _FIXED_H
#define _FIXED_H
#include<alloc.h>
struct fsp_t //fixed-size partation
{
	void *head;
};

struct fsp_t fs_init(void *ptr,size_t block,size_t num);
void* fs_alloc(struct fsp_t *f);
void fs_free(struct fsp_t *f,void *p);

#endif

