#include<task.h>
#include<trap.h>
#include<ds.h>
#include<prio.h>
#include<bits.h>

//scheduler 
struct prio sched={0,{0}};


//time blocker
struct heap slp_q={{0},0};


//task control block
static tcb_t Task_Tcb[TASK_NUM]={[0]={.prio=63,.status=1,.task_name="main"}}; 
static char  Task_Tcb_Used[TASK_NUM]={1};
static tid_t Task_Tcb_Index=1;
static tid_t Task_Counter=1;


static struct task_alloc_t
{
	tcb_t task_list[64];
	uint64_t task_bits;
	uint64_t task_size;
} task_pool ={{[0]={.prio=63,.task_name="main"}},1,1} ;

static tid_t tid_alloc(struct task_alloc_t *pool)
{
	uint64_t b=~pool->task_bits;
	int pos=ntz(b);
	pool->task_bits=bit_set(pool->task_bits,pos);
	pool->task_size+=1;
	return pos;
}

static void tid_free(struct task_alloc_t *pool,int pos)
{
	pool->task_bits=bit_clr(pool->task_bits,pos);
	pool->task_size-=1;
}


//point to task 
static tid_t Task_Id=0;
static tid_t Task_Next_Id=0;

tid_t task_add(void (*task)(),task_attr_t attr)
{
	tid_t id=tid_alloc(&task_pool);
	task_pool.task_list[id]=(tcb_t){
										.sp=attr.sp-32,
										.prio=attr.prio,
										.id=id,
										.excutable_time=0,
										.task_name=attr.task_name,
										
										
									};
	//for(int i=0;i<32;i++)
	//	(attr.sp-32)[i]=0;
	gprs_t *gprs=(void*)(attr.sp-32);
	gprs->sp=(reg_t)gprs;
	extern void task_del();
	gprs->ra=(reg_t)task_del;
	
	return id;
}

static void task_store_context(tid_t id,reg_t sp,reg_t epc)
{
	task_pool.task_list[id].sp=sp;
	task_pool.task_list[id].pc=epc;
}

void task_del(tid_t id)
{
	tid_free(&task_pool,id);
}

reg_t __attribute__((weak)) task_switch(tid_t id)
{
	task_store_context(id,0,read_csr(mepc));
	Task_Id=id;
	write_csr(mepc,task_pool.task_list[id].pc);
	return (reg_t)task_pool.task_list[id].sp;
}

//initialize context of task

void task_exit()
{
	xlen_t sr;
	atomic_begin(sr);
	printf("task return \n");
	Task_Tcb_Used[get_tid()]=0;
	prio_del(&sched,get_tprio(get_tid()));
	tid_t id=prio_tid(&sched);
	if(id<0)
		id=0;
	task_next_task(id);
	atomic_end(sr);
	task_sche();
}


static xlen_t* task_stk_init(void *task,xlen_t *sp,void *exit_handler)
{
	xlen_t *stk=sp-32; //alloc 32*xlen size of spaces
	stk[0]=(xlen_t)(exit_handler?exit_handler:task_exit); //ra
#ifdef INIT_CTX_ALL
	stk[1]=0;//unused
	stk[2]=0;//unused
	stk[3]=0;//unused
	stk[4]=0;
	stk[5]=0;
	stk[6]=0;
	stk[7]=0;
	stk[8]=0;
	stk[9]=0;
	stk[10]=0;
	stk[11]=0;
	stk[12]=0;
	stk[13]=0;
	stk[14]=0;
	stk[15]=0;
	stk[16]=0;
	stk[17]=0;
	stk[18]=0;
	stk[19]=0;
	stk[20]=0;
	stk[21]=0;
	stk[22]=0;
	stk[23]=0;
	stk[24]=0;
	stk[25]=0;
	stk[26]=0;
	stk[27]=0;
	stk[28]=0;
	stk[29]=0;
	stk[30]=0;
#endif
	stk[31]=(xlen_t)task;//mepc                      
    return stk;
}

static void next_Index(void)
{
	if(Task_Tcb_Index<TASK_NUM-1)
		Task_Tcb_Index++;
	else 
		Task_Tcb_Index=0;
}

tid_t task_creat(void (*task)(),task_attr_t attr)
{
	xlen_t sr;
	atomic_begin(sr);
	if(Task_Counter>=TASK_NUM)
	{
		printf("Task full\n");
		exit(1);
	}
	
	while(Task_Tcb_Used[Task_Tcb_Index]!=0)
		next_Index();
	Task_Counter++;
	Task_Tcb_Used[Task_Tcb_Index]=1;
	
	Task_Tcb[Task_Tcb_Index].sp=task_stk_init(task,attr.sp,attr.exit_handler);
	Task_Tcb[Task_Tcb_Index].task_name=attr.task_name;
	Task_Tcb[Task_Tcb_Index].status=NEW;
	Task_Tcb[Task_Tcb_Index].mgr=attr.mgr?attr.mgr:Task_Id;
	Task_Tcb[Task_Tcb_Index].prio=attr.prio;

	prio_add(&sched,Task_Tcb_Index,attr.prio);
	
	//task_awake(Task_Tcb_Index);
	
	
	tid_t index=Task_Tcb_Index;
	printf("task id =%lld \n",index);
	atomic_end(sr);
	return index;
}



void task_block()
{
	xlen_t sr;
	atomic_begin(sr);
	Task_Tcb[get_tid()].status&=~1;
	prio_del(&sched,Task_Tcb[get_tid()].prio);
	int id=prio_tid(&sched);
	task_next_task(id);
	atomic_end(sr);
	task_sche();
}

void task_awake(tid_t id)
{
	xlen_t sr;
	atomic_begin(sr);
	Task_Tcb[id].status|=1;
	prio_add(&sched,id,Task_Tcb[id].prio);
	id=prio_tid(&sched);
	atomic_end(sr);
	if(id>=0)
	{
		task_next_task(id);
		task_sche();
	}
	
}

void task_set_status(tid_t id,int status)
{
	xlen_t sr;
	atomic_begin(sr);
	Task_Tcb[id].status &=1;
	Task_Tcb[id].status |=status<<1;
	atomic_end(sr);
}

int task_get_status(tid_t id)
{
	xlen_t sr;
	atomic_begin(sr);
	int r=Task_Tcb[id].status;
	atomic_end(sr);
	return r;
}

//get tcb information
//these function are used by .s file
tid_t get_tid()
{
	return Task_Id;
}

tid_t get_mgr(tid_t id)
{
	return Task_Tcb[id].mgr;
}

tid_t get_next_tid()
{
	return Task_Next_Id;
}

tid_t set_tid(tid_t id)
{
	Task_Id=id;
}

xlen_t get_tcb_sp(tid_t id)
{
	return (xlen_t)&Task_Tcb[id].sp;
}

char* get_task_name(tid_t id)
{
	return Task_Tcb[id].task_name;
}

uint64_t get_finish_time(tid_t id)
{
	return Task_Tcb[id].finish_time;
}

int get_tprio(tid_t id)
{
	return Task_Tcb[id].prio;
}

//switch context of task
void task_next_task(tid_t id)
{
	xlen_t sr;
	atomic_begin(sr);
	Task_Next_Id=id;
	atomic_end(sr);
}

void task_sche(void)
{
	if(Task_Next_Id == Task_Id)
		return ;

	system_call(SYS_TASKSW);
}



void task_sleep(uint64_t t)
{
	xlen_t sr;
	atomic_begin(sr);
	
	Task_Tcb[Task_Id].finish_time=get_time()+t;
	heap_push(&slp_q,Task_Id);
	prio_del(&sched,Task_Tcb[Task_Id].prio);
	
		
	while(get_time()<Task_Tcb[Task_Id].finish_time)
	{
		int pr=prio_get(&sched);
		if(pr>=0)
		{
			task_next_task(prio_tid(&sched));
			atomic_end(sr);
			task_sche();
			return ;
		}
	}
	atomic_end(sr);
}

