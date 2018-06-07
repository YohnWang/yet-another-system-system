#include<task.h>
#include<trap.h>

//task control block
static tcb_t Task_Tcb[TASK_NUM]; 
static char  Task_Tcb_Used[TASK_NUM]={1};
static tid_t Task_Tcb_Index=1;
static tid_t Task_Counter=1;

//point to task 
static tid_t Task_Id=0;
static tid_t Task_Next_Id=0;

//initialize context of task

static void task_exit_handler()
{
	printf("task return \n");
	tid_t id=get_mgr(get_tid());
	Task_Tcb_Used[get_tid()]=0;
	task_next_task(id);
	task_sche();
}


static xlen_t* task_stk_init(void *task,xlen_t *sp,void *exit_handler)
{
	xlen_t *stk=sp-32; //alloc 32*xlen size of spaces
	stk[0]=(xlen_t)(exit_handler?exit_handler:task_exit_handler); //ra
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

	
	volatile tid_t index=Task_Tcb_Index;
	printf("task id =%lld \n",index);
	atomic_end(sr);
	return index;
}



void task_block(tid_t id)
{

}

void task_awake(tid_t id)
{

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
	//if(get_time()<Task_Tcb[Task_Next_Id].finish_time)
	//	return ;
	system_call(SYS_TASKSW);
}

static tid_t find_first_task(tid_t now)
{
	now++;
	if(now>=TASK_NUM)
		now=0;
	while(Task_Tcb_Used[now]==0)
	{
		now++;
		if(now>=TASK_NUM)
			now=0;
	}
}

void task_sleep(uint64_t t)
{
	xlen_t sr;
	atomic_begin(sr);
	Task_Tcb[Task_Id].finish_time=get_time()+t;
	atomic_end(sr);
	if(Task_Next_Id!=Task_Id)
		task_sche();
	while(get_time()<Task_Tcb[Task_Id].finish_time)
	{
		task_next_task(find_first_task(Task_Id));
		task_sche();
	}
}

