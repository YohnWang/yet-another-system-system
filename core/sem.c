#include<cpu.h>
#include<sem.h>

void sem_wait(sem *s)
{
	xlen_t sr;
	atomic_begin(sr);
	if(s->counter<=0)
	{
		s->wait_arr[s->wait_num++]=get_tid();
		
		atomic_end(sr);
		task_block();
		atomic_begin(sr);
		s->counter--;
		atomic_end(sr);
		return ;
	}
	s->counter--;
	atomic_end(sr);
}

static int find_high_task(sem *s)
{
	int i=0;
	int pr=65535,r=-1;
	for(i=0;i<s->wait_num;i++)
	{
		if(get_tprio(s->wait_arr[i])<pr)
		{
			pr=get_tprio(s->wait_arr[i]);
			r=i;
		}
	}
	return r;
}

static void arr_del(tid_t a[],int n,int i)
{
	for(int t=i;t<n-1;t++)
		a[t]=a[t+1];
}

void sem_signal(sem *s)
{
	xlen_t sr=cpu_sr_set();
	s->counter++;
	int dx=find_high_task(s);
	tid_t id;
	if(dx!=-1)
	{
		id=s->wait_arr[dx];
		arr_del(s->wait_arr,s->wait_num--,dx);
	}
	cpu_sr_reset(sr);
	task_awake(id);
}
