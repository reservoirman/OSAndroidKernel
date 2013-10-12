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
    
          if (sem->sthread_count == 0) {
	       sem->head = (struct Node*) malloc((size_t)sizeof(struct Node));
	       sem->start = sem->head ;
               sem->head->data = data;
               sem->head->next = NULL;
               sem->sthread_count++;
                
           } 
           else 
	   {
		struct Node *temp = (struct Node *) malloc((size_t)sizeof(struct Node));
                sem->head->next = temp;
		temp->data = data;
                temp->next = NULL;
                sem->head = temp;
                sem->sthread_count++;
                
           }
}

struct Node * removeNode(struct sthread_sem_struct *sem) 
    {
    struct Node *temp;
    temp = sem->start;
    sem->start = sem->start->next;
    sem->sthread_count--;
    return temp;
}

/*void traverse(struct sthread_sem_struct *sem) {
   	struct Node *current;
    	current = sem->start;
	if (current == NULL)
	{
		printf("Queue is empty!");
	}
   	 while(current != NULL) 
	{
        printf("current data: %p current node: %p next node: %p\n", current->data, current, current->next);
        current = current->next;
    	}
	printf("\n");
}
*/

void lock()
{
	while (test_and_set(&flag))
		;
}
void unlock()
{
	flag=0;
}

int sthread_sem_init(sthread_sem_t *sem, int count)
{
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
	
	
	lock();	
	
	int ret;
	
        if (sem->count > 0)
	{
	 
            sem->count--;
	    ret = 0;
	    unlock();

	}	
	else
	{
	 
          insertNode(sem, sthread_self());
	  ret = -1;
	  unlock();
	  sthread_suspend();  
        } 
	
	return ret;
       
}

int sthread_sem_try_down(sthread_sem_t *sem)
{
	
	lock();
	int ret;
	if (sem->count > 0)
	{
		sem->count--;
		ret = 0;
	}
	else
	{
		ret = -1;
	}
	unlock();
        return ret;
}


int sthread_sem_up(sthread_sem_t *sem)
{
	
	lock();
	
	
	if (sem->sthread_count <= 0)
	{
		sem->count++;
		unlock();
	}
	else	
	{	
		if(sem->count == 0)
			sem->count++;
		struct Node *longestWaitingThread = removeNode(sem);
		struct Node temp;
		temp.data = longestWaitingThread->data;
		free(longestWaitingThread);
		unlock();
		sthread_wake(temp.data);
		
		
	
	}
	
	return 0;
}       



