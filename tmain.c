#include"utils.h"

#define STK_LEN 4096

xlen_t Task1_STK[STK_LEN];
xlen_t Task2_STK[STK_LEN];
xlen_t Task3_STK[STK_LEN];
xlen_t Task4_STK[STK_LEN];

tid_t t1,t2,t3,t4;

void task1()
{
	int cnt=0;
	for(;;)
	{
		xlen_t sr;
		atomic_begin(sr);
		printf("task1 is runing %d\n",cnt++);
		atomic_end(sr);
		task_sleep(1000000);
		//task_next_task(t2);
		//task_sche();
	}
}

void task2()
{
	int x=0;
	for(;;)
	{
		xlen_t sr;
		atomic_begin(sr);
		printf("task2 is runing %d\n",x++);
		atomic_end(sr);
		task_sleep(1000000);
		//task_next_task(t1);
		//task_sche();
	}
}

void task3()
{
	int cnt=0;
	for(;;)
	{
		xlen_t sr;
		atomic_begin(sr);
		printf("task3 is runing %d\n",cnt++);
		atomic_end(sr);
		task_sleep(1000000);
		//task_next_task(0);
		//task_sche();
	}
}

void task4()
{
	int x=0;
	for(;;)
	{
		xlen_t sr;
		atomic_begin(sr);
		printf("task4 is runing %d\n",x++);
		atomic_end(sr);
		task_sleep(1000000);
		//task_next_task(0);
		//task_sche();
	}
}


int main(int argc,char *argv[])
{
	(void)argc,(void)argv;
	printf("start...\n");
	/*
	int fcnt=0,gcnt=0;
	for(;;)
	{
		int *p=mm_malloc(1024);
		//p[100]=1;
		if(p==NULL)
			printf("fuck %d\n",fcnt++);
		else 
			printf("good %d\n",gcnt++);
		if((intptr_t)p%3)
			mm_free(p);
		delay(1000000);
	}
	
	
	for(;;){}*/
	
	/*
	void *p=mm_malloc(1024);
	struct fsp_t f=fs_init(p,8,32);
	int *fp=fs_alloc(&f);
	*fp=124;
	for(;;){}
	*/
	t1=task_creat(task1,(task_attr_t){&Task1_STK[STK_LEN-1],NULL,"task1"});
	t2=task_creat(task2,(task_attr_t){&Task2_STK[STK_LEN-1],NULL,"task2"});
	t3=task_creat(task3,(task_attr_t){&Task3_STK[STK_LEN-1],NULL,"task3"});
	t4=task_creat(task4,(task_attr_t){&Task4_STK[STK_LEN-1],NULL,"task4"});
	//task_next_task(t1);
	//task_sche();
	
	//rr_add(t1);
	//rr_add(t2);
	rr_add(t3);
	rr_add(t4);
	//rr_del(t4);
	enable_time_int();
	enable_global_int();
	task_next_task(t1);
	task_sche();
	while(1)
	{
		tid_t id=rr_next();
		//printf("id=%ld\n",id);
		task_next_task(id);
		task_sche();
	}
	
	
	return 0;
}


void printx(xlen_t x)
{
	xlen_t sr=cpu_sr_set();
	printf("xreg is %llx\n",x);
	cpu_sr_reset(sr);
}
