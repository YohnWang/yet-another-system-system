#include<task.h>


//task control block
static tcb_t Task_Tcb[TASK_NUM]; 
static char  Task_Tcb_Used[TASK_NUM]={1};
static tid_t Task_Tcb_Index=1;
static tid_t Task_Counter=1;

//ready queue
//TODO (heap,tree,or queue)
struct 
{
	tid_t arr[TASK_NUM];
	tid_t size;
	tid_t front;
	tid_t rear;
} Task_Rdy_Queue={{},0,0,0};

tid_t Task_Rdy_Index=0;
tid_t Task_Id=0;
tid_t Task_Next_Id=0;


static void task_add(tid_t id)
{
	tid_t s=Task_Rdy_Queue.size++;
	Task_Rdy_Queue.arr[s]=id;
}

static void task_del(tid_t id)
{
	
}



xlen_t* task_stk_init(void *task,xlen_t *sp,void *exit_handler)
{
	xlen_t *stk=sp-32;
	stk[0]=(xlen_t)exit_handler; //ra
	stk[1]=0x80;//mstatus.set 0x80 , not used
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
	xlen_t sr=cpu_sr_set();
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
	

	
	volatile tid_t index=Task_Tcb_Index;
	printf("task id =%lld \n",index);
	cpu_sr_reset(sr);
	return index;
}

tid_t get_tid()
{
	return Task_Id;
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


void task_next_task(tid_t id)
{
	Task_Next_Id=id;
}

void task_sche()
{
	//xlen_t sr=cpu_sr_set();
	asm("scall");
	
	//cpu_sr_reset(sr);
}



