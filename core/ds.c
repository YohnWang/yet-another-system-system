#include<ds.h>



#define PARENT(i)  ((i-1)/2)
#define LEFT(i)    (i*2+1)
#define RIGHT(i)   (i*2+2)

static inline int Etype_less(Etype a,Etype b)
{
    return get_finish_time(a)<get_finish_time(b);
}

int heap_empty(struct heap *h)
{
	return !h->size;
}

int heap_full(struct heap *h)
{
	return h->size==sizeof(h->a)/sizeof(h->a[0]);
}

void heap_push(struct heap *h,Etype x)
{
    if(heap_full(h))
    {
        printf("Error, heap is full ,but push\n");
        exit(1);
    }
    h->size++;
    int end=h->size-1;
    int i;
    for(i=end;i!=PARENT(i);i=PARENT(i))
    {
        if(Etype_less(x,h->a[PARENT(i)]))
            h->a[i]=h->a[PARENT(i)];
        else
            break;
    }
    h->a[i]=x;

}

Etype heap_pop(struct heap *h)
{
    if(heap_empty(h))
    {
        printf("Error, heap is empty ,but pop\n");
        exit(1);
    }
    Etype ret=h->a[0];
    h->a[0]=h->a[--h->size];
    int i=0;
    for(;;)
    {
        int l=LEFT(i);
        int r=RIGHT(i);
        int largest=i;
        if(l<h->size&&Etype_less(h->a[l],h->a[largest]))
            largest=l;
        if(l<h->size&&Etype_less(h->a[r],h->a[largest]))
            largest=r;
        if(largest!=i)
        {
            Etype tmp=h->a[i];
            h->a[i]=h->a[largest];
            h->a[largest]=tmp;
            i=largest;
        }
        else
            break;
    }
    return ret;
}

