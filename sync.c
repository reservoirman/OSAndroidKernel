/*
 * NAME, etc.
 *
 * sync.c
 *
 * Synchronization routines for SThread
 */

#define _REENTRANT

#include "sthread.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * semaphore routines
 */

unsigned long int flag=0;

void insertNode(struct sthread_sem_struct *sem, sthread_t data) {
    
          if (sem->sthread_count == 0) 
	  {
	       sem->start = sem->head ;
		
	       sem->head = (struct Node*) malloc((size_t)sizeof(struct Node));
               sem->head->data = data;
 
               sem->head->next = NULL;

               sem->sthread_count++;

               printf("data:%p node:%p next:%p cnt:%d\n", sem->head->data, sem->head, sem->head->next, sem->sthread_count);
           } 
           else 
	   {
		struct Node *temp = (struct Node *) malloc((size_t)sizeof(struct Node));
                sem->head->next = temp;
		temp->data = &data;
                temp->next = NULL;
                sem->head = temp;
                sem->sthread_count++;	
           }
}


void lock()
{
	while (test_and_set(&flag))
		sched_yield();
}
void unlock()
{
	flag=0;
}

int sthread_sem_init(sthread_sem_t *sem, int count)
{
	/* FILL ME IN! */
	lock();
        sem->count = count;
        sem->head = NULL;
	sem->start = NULL ;
        sem->sthread_count = 0;
        int ret;
        if ((sem->count == count) && (sem->head == NULL))
	{
	    
            ret =  0;
	}        
	else 
	{	
	    ret = -1;
	}
        unlock();

	return ret;
}

int sthread_sem_destroy(sthread_sem_t *sem)
{
	/* FILL ME IN! */
	lock();
        sem->count = 0;
        sem->head = NULL;
	int ret;
	if ((sem->count == 0) && (sem->head == NULL))
	{
	    
            ret =  0;
	}        
	else 
	{	
	    ret = -1;
	}
        unlock();
        return ret;
}

int sthread_sem_down(sthread_sem_t *sem)
{
	printf("semcnt:%d\n", sem->count);
	/* FILL ME IN! */
	lock();
	int ret;
        if (sem->count > 0)
	{
	printf("fi \n");
            sem->count--;
	    ret = 0;
	}	
	else
	{
	printf("hi \n");
          insertNode(sem, sthread_self());
	  sthread_suspend();  // causes segfault
	   ret = -1;
        } 
	unlock();
	return ret;
       
}

int sthread_sem_try_down(sthread_sem_t *sem)
{
	/* FILL ME IN! */
        return -1;
}

int sthread_sem_up(sthread_sem_t *sem)
{
	/* FILL ME IN! */
        return -1;
}

