#include<fixed.h>

static void insert(void **head,void *ptr)
{
	void *next=*head;
	*(void**)ptr=next;
	*head=ptr;
}

struct fsp_t fs_init(void *ptr,size_t block,size_t num) 
{
	if((block&7)!=0) //block must be 8 times
		return (struct fsp_t){NULL};
	struct fsp_t f;
	f.head=NULL;
	for(size_t i=0;i<num;i++)
	{
		insert(&f.head,(int8_t*)ptr+i*block);
	}
	return f;
}

void* fs_alloc(struct fsp_t *f)
{
	void *r=f->head;
	if(r!=NULL)
		f->head=*(void**)r;
	return r;
}

void fs_free(struct fsp_t *f,void *p)
{
	if(p==NULL)
		return ;
	*(void**)p=f->head;
	f->head=p;
}
