#include"utils.h"

#define STK_LEN 4096

xlen_t Task1_STK[STK_LEN];
xlen_t Task2_STK[STK_LEN];
xlen_t Task3_STK[STK_LEN];
xlen_t Task4_STK[STK_LEN];
xlen_t Task5_STK[STK_LEN];

void task()
{
	int cnt=0;
	//for(;;)
	{
		printf("%s is runing %d\n",get_task_name(get_tid()),cnt++);
		//task_next_task(0);
		//task_sche();
		delay(100000);
	}
}

void timer()
{
	uint64_t next_time=get_time()+1000000;
	for(;;)
	{
		if(next_time<=get_time())
		{
			printf("timer occur\n");
			next_time=get_time()+1000000;
		}
		task_next_task(0);
		task_sche();
	}
}


void thread()
{
	
}

int main()
{
	tid_t t1,t2,t3,t4,t5;
	t1=task_creat(task,(task_attr_t){&Task1_STK[STK_LEN-1],.task_name="task1"});
	t2=task_creat(task,(task_attr_t){&Task2_STK[STK_LEN-1],.task_name="task2"});
	t3=task_creat(task,(task_attr_t){&Task3_STK[STK_LEN-1],.task_name="task3"});
	t4=task_creat(task,(task_attr_t){&Task4_STK[STK_LEN-1],.task_name="task4"});
	t5=task_creat(timer,(task_attr_t){&Task5_STK[STK_LEN-1],.task_name="timer"});
	prio cap={0,{0}};
	prio_add(&cap,t1,1);
	prio_add(&cap,t2,3);
	prio_add(&cap,t3,4);
	prio_add(&cap,t4,6);
	for(;;)
	{
		int   pr=prio_get(&cap);
		if(pr<0)
			return 2;
		tid_t id=prio_tid(&cap);
		task_next_task(id);
		task_sche();
		prio_del(&cap,pr);
		
		
	}
}
