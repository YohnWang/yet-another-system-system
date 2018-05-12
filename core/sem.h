#ifndef _SEM_H
#define _SEM_H

typedef int sem;
void wait(sem *s);
void signal(sem *s);


#endif
