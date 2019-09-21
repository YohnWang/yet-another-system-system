#include"utils.h"

#define QSIZE (20)
struct queue
{
	int a[QSIZE];
	int size;
	int front;
	int rear;
} que={0};

void queue_push(int x)
{
	if(que.size<QSIZE)
	{
		int rear=(que.rear+1)%QSIZE;
		que.a[que.rear]=x;
		que.size++;
		que.rear=rear;
	}
}

int queue_pop()
{
	int r=-1;
	if(que.size>=0)
	{
		r=que.a[que.front];
		int front=(que.front+1)%QSIZE;
		que.front=front;
		que.size--;
	}
	return r;
}














#define STK_LEN 2048*4

xlen_t Task1_STK[STK_LEN];
xlen_t Task2_STK[STK_LEN];
xlen_t Task3_STK[STK_LEN];
xlen_t Task4_STK[STK_LEN];
xlen_t Task5_STK[STK_LEN];

tid_t t1,t2,t3,t4,t5;

sem full={0};
sem empty={10};
sem mutex={1};

int index=0;

void producer()
{
	int cnt=0;
	for(;;)
	{
		sem_wait(&empty);
		sem_wait(&mutex);
		
		queue_push(cnt);
		printf("produce %d...\n",cnt);
		cnt++;
		
		sem_signal(&mutex);
		sem_signal(&full);
		task_sleep(100000);
	}
}

void customer()
{
	for(;;)
	{
		sem_wait(&full);
		sem_wait(&mutex);
		
		printf("custom %d...\n",queue_pop());
		
		sem_signal(&mutex);
		sem_signal(&empty);
		task_sleep(1000000);
	}
}


#include<encoding.h>
int main()
{
	
	t1=task_creat(producer,(task_attr_t){&Task1_STK[STK_LEN-1],1, .task_name="task1"});
	t2=task_creat(customer,(task_attr_t){&Task2_STK[STK_LEN-1],2, .task_name="task2"});
	//t3=task_creat(task,(task_attr_t){&Task3_STK[STK_LEN-1],3, .task_name="task3"});
	//t4=task_creat(task,(task_attr_t){&Task4_STK[STK_LEN-1],4, .task_name="task4"});
	//t5=task_creat(timer,(task_attr_t){&Task5_STK[STK_LEN-1],5,.task_name="timer"});
	//enable_global_int();
	//enable_time_int();
	//task_sleep(500000);
	printf("\n%ld\n",rdtime());
	exit(0);
	for(;;){}
}
