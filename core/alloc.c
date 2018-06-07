#include<alloc.h>

extern char _heap_start[];
extern char _heap_end[];

static char *mem_heap;
static char *mem_brk;
static char *mem_max_addr; //saved.
static void *first_unalloc;

/*
linked list
status=1 : allocated
status=0 : unallocated
size includes mm_info size 
*/

struct mm_info
{
	struct mm_info *prev;
	struct mm_info *next;
	size_t size;
	long status;
};
 



#define CEIL8(x) (((x)+7)&-8)
#define FLOOR8(x) ((x)&-8)

//this function must be used in init.c
void init_heap(void)
{
	mem_heap=(void*)CEIL8((uintptr_t)_heap_start);
	mem_brk=(void*)FLOOR8((uintptr_t)_heap_end);
	mem_max_addr=_heap_end;
	
	struct mm_info * p=(void*)mem_heap;
	p->prev=NULL;
	p->next=NULL;
	p->size=mem_brk-mem_heap;
	p->status=0;
	first_unalloc=p;
}

static void* find_fit(size_t asize) //first fit
{
	struct mm_info *p=(void*)first_unalloc;
	while(p!=NULL &&( p->status!=0 || p->size < asize ))
	{
		p=p->next;
	}
	return p;
}


static void place(struct mm_info *bp,size_t asize)
{
	struct mm_info *p=bp;
	struct mm_info *new;

	size_t old_size=p->size;
	size_t new_size=old_size-asize;

	p->status=1;

	if(new_size > sizeof(struct mm_info) + sizeof(long))
    {
        p->size=asize;

        new=(void*)((char*)bp+asize);
        new->next=p->next;
        p->next=new;
        new->prev=p;
        new->size=new_size;
        new->status=0;
        first_unalloc=new;
    }
	else
    {
        //just alloc
    }
}

void* mm_malloc(size_t size)
{
	size_t asize; //adjusted block size 
	
	if(size == 0)
		return NULL;
	
	asize=CEIL8(size); //aligned 8
	
	asize+=sizeof(struct mm_info); //alloc mm_info block
	
	struct mm_info *bp=find_fit(asize);
	if(bp!=NULL)
	{
		place(bp,asize);
		return bp+1; //get block's pointer
	}
	
	//TODO 
	//extend brk
	return NULL;
	
}

static inline int is_alloc(struct mm_info *p)
{
	if(p==NULL || p->status!=0)
		return 1;
	return 0;	
}

void mm_free(void *bp)
{
	if(bp==NULL)
		return ;
	struct mm_info *p=bp;
	p=p-1; //get mm information
	
	struct mm_info *prev=p->prev;
	struct mm_info *next=p->next;
	
	int prev_alloc=is_alloc(prev);
	int next_alloc=is_alloc(next);
	
	if(prev_alloc && next_alloc ) //alloc<->free<->alloc
	{
		p->status=0;
		first_unalloc=p;
	}
	else if(prev_alloc && !next_alloc ) //alloc<->free<->unalloc
	{
		p->size+=next->size;
		p->next=next->next;
		
		if(next->next!=NULL)
		{
			next->next->prev=p;
		}
		p->status=0;
		first_unalloc=p;
	}
	else if(!prev_alloc && next_alloc) //unalloc<->free<->alloc
	{
		prev->size+=p->size;
		prev->next=p->next;
		
		if(p->next!=NULL)
		{
			p->next->prev=prev;
		}
		first_unalloc=prev;
	}
	else  //unalloc<->free<->unalloc
	{
		prev->size+=p->size + next->size;
		prev->next=next->next;
		
		if(next->next!=NULL)
		{
			next->next->prev=prev;
		}
		first_unalloc=prev;
	}
}


