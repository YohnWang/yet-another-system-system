#include"utils.h"

#define STK_LEN 4096

xlen_t Task1_STK[STK_LEN];
xlen_t Task2_STK[STK_LEN];
xlen_t Task3_STK[STK_LEN];
xlen_t Task4_STK[STK_LEN];
xlen_t Task5_STK[STK_LEN];

tid_t t1,t2,t3,t4,t5;
sem s={0};
void task()
{
	int cnt=0;
	for(;;)
	{
		xlen_t sr;
		//atomic_begin(sr);
		//printf("%s is runing %d\n",get_task_name(get_tid()),cnt++);
		//atomic_end(sr);
		//task_sleep(1000000);
		sem_wait(&s);
		atomic_begin(sr);
		printf("fuck yourself\n");
		atomic_end(sr);
	}
}

void exec()
{
	for(;;)
	{
		xlen_t sr;
		atomic_begin(sr);
		printf("signal \n");
		atomic_end(sr);
		
		sem_signal(&s);
		task_sleep(1000000);
	}
}

void timer()
{
	//uint64_t next_time=get_time()+1000000;
	for(;;)
	{
		task_sleep(12345000);
		xlen_t sr;
		atomic_begin(sr);
		printf("timer occur\n");
		
		atomic_end(sr);task_awake(t1);
		
	}
}


void thread()
{
	
}

int main()
{
	
	t1=task_creat(task,(task_attr_t){&Task1_STK[STK_LEN-1],1, .task_name="task1"});
	t2=task_creat(exec,(task_attr_t){&Task2_STK[STK_LEN-1],2, .task_name="task2"});
	//t3=task_creat(task,(task_attr_t){&Task3_STK[STK_LEN-1],3, .task_name="task3"});
	//t4=task_creat(task,(task_attr_t){&Task4_STK[STK_LEN-1],4, .task_name="task4"});
	//t5=task_creat(timer,(task_attr_t){&Task5_STK[STK_LEN-1],5,.task_name="timer"});
	prio cap={0,{0}};
	//prio_add(&cap,t1,1);
	//prio_add(&cap,t2,3);
	//prio_add(&cap,t3,4);
	//prio_add(&cap,t4,6);
	enable_global_int();
	enable_time_int();
	extern struct prio sched;
	for(;;){/*if(sched.prio_tab)printf("%lld\n",sched.prio_tab);*/}
	for(;;)
	{
		int   pr=prio_get(&cap);
		//if(pr<0)
		//	return 2;
		tid_t id=prio_tid(&cap);
		task_next_task(id);
		task_sche();
		prio_del(&cap,pr);
		
		
	}
}
