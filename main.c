#include"utils.h"

#define STK_LEN 4096

xlen_t Task1_STK[STK_LEN];
xlen_t Task2_STK[STK_LEN];

tid_t t1,t2;

void task1()
{
	for(;;)
	{
		printf("task1 is runing\n");
		task_next_task(t2);
		task_sche();
	}
}

void task2()
{
	for(;;)
	{
		printf("task2 is runing\n");
		task_next_task(t1);
		task_sche();
	}
}


int main(int argc,char *argv[])
{
	(void)argc,(void)argv;
	t1=task_creat(task1,(task_attr_t){&Task1_STK[STK_LEN-1],NULL,"task1"});
	t2=task_creat(task2,(task_attr_t){&Task2_STK[STK_LEN-1],NULL,"task2"});
	task_next_task(t1);
	task_sche();
	//enable_time_int();
	//enable_global_int();
	/*
	_auto x=get_time();
	_auto y=x;
	while(get_time()-x<10000)
	{
		if(get_time()!=y)
		{
			y=get_time();
			xlen_t sr=cpu_sr_set();
			printf("%lld\n",y);
			cpu_sr_reset(sr);
			
		}
	}*/
	
	return 0;
}


void printx(xlen_t x)
{
	xlen_t sr=cpu_sr_set();
	printf("xreg is %llx\n",x);
	cpu_sr_reset(sr);
}
