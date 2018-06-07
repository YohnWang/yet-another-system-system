#include<rrsched.h>
#include<cpu.h>


typedef struct list list;
struct list
{
    list *prev;
    list *next;
};

static void list_init(list *p)
{
    p->prev=p;
    p->next=p;
}

static void list_insert(list *p,list *new)
{
    list *p_next;
    p_next=p->next;
    p->next=new;
    p_next->prev=new;
    new->next=p_next;
    new->prev=p;
}

static void list_del(list *entry)
{
    list *prev,*next;
    prev=entry->prev;
    next=entry->next;
    prev->next=next;
    next->prev=prev;
}


struct list_task
{
	list list;
	tid_t id;
};
typedef struct list_task list_task;

static list_task rdy_list[TASK_NUM];
static list_task *head_of_list=0;


void rr_add(tid_t id)
{
	rdy_list[id].id=id;
	if(head_of_list==0)
	{
		list_init(&rdy_list[id].list);
		head_of_list=&rdy_list[id];
	}
	else 
	{
		list_insert(&head_of_list->list,&rdy_list[id].list);
	}
}


void rr_del(tid_t id)
{
	list_task *p=head_of_list;
	if(head_of_list->id==id)
		head_of_list=(list_task*)head_of_list->list.next;
	if(p==head_of_list)
		head_of_list=0;
	else 
		list_del(&rdy_list[id].list);
}

tid_t rr_next(void)
{
	list_task *p=head_of_list;
	if(p==0)
		return 0;
	p=(list_task*)p->list.next;
	tid_t r=p->id;
	head_of_list=p;
	return r;
}

