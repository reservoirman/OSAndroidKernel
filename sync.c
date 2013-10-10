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
printf("MALLOC:%p\n", sem->head);
	       sem->start = sem->head ;
               sem->head->data = data;
               sem->head->next = NULL;
               sem->sthread_count++;
                
           } 
           else 
	   {
		struct Node *temp = (struct Node *) malloc((size_t)sizeof(struct Node));
printf("MALLOC:%p\n", temp);
                sem->head->next = temp;
		temp->data = data;
                temp->next = NULL;
                sem->head = temp;
                sem->sthread_count++;
                
           }
	   printf("InsertNode: data:%p node:%p next:%p cnt:%d\n", sem->head->data, sem->head, sem->head->next, sem->sthread_count);
}

struct Node * removeNode(struct sthread_sem_struct *sem) {
    struct Node *temp;
    temp = sem->start;
    sem->start = sem->start->next;
	sem->sthread_count--;
                printf("RemoveNode: data:%p node:%p next:%p cnt:%d\n", temp->data, temp, temp->next, sem->sthread_count);
   
    // wake up node
    // potentially free it
	return temp;
}

void traverse(struct sthread_sem_struct *sem) {
    struct Node *current;
    current = sem->start;
	printf("Traversing through the queue: \n");
	if (current == NULL)
	{
		printf("Queue is empty!");
	}
    while(current != NULL) {
        printf("current data: %p current node: %p next node: %p\n", current->data, current, current->next);
        current = current->next;
    }
	printf("\n");
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
	
	/* FILL ME IN! */
	lock();
	traverse(sem);
	printf("semcnt:%d\n", sem->count);
	int ret;
	//sem->count--;
        if (sem->count > 0)
	{
	    printf("DOWN (available)! \n");
            sem->count--;
	    ret = 0;
	    unlock();
	}	
	else
	{
	  printf("DOWN (unavailable)! \n");
          insertNode(sem, sthread_self());
	  ret = -1;
	  unlock();
	  sthread_suspend();  // causes segfault
        } 
	
	return ret;
       
}

int sthread_sem_try_down(sthread_sem_t *sem)
{
	/* FILL ME IN! */
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
	/* FILL ME IN! */
	
	
	 
	
	lock();
	traverse(sem);
	printf("UP! ( semaphore released by calling thread )\n");
	
	//if there are threads waiting
	if (sem->sthread_count <= 0)
	{
		sem->count++;
		unlock();
	}
	else	
	{
		struct Node *longestWaitingThread = removeNode(sem);
		//sthread_t temp = longestWaitingThread->data;
		//free(longestWaitingThread);
		//printf("FREE:%p\n", longestWaitingThread);
		//printf("temp:%p\n",temp);
		
		sthread_wake(longestWaitingThread->data);
		unlock();
		//that thread's been removed from the queue; now just free it
	
	}
	
	return 0;
}       

//this is a quick mini test that demonstrates the working-ness of the linked list
void queueTest()
{
	sthread_sem_t holla;
	
	sthread_sem_init(&holla, 0);
		
	int i = 0;
	int total = 1;
	for (i = 0; i < total; i++)
	{
		
		sthread_t thread = sthread_self();
		insertNode(&holla, thread );
		
	}
	//should display 4 nodes, each one containing the same thread as data
	traverse(&holla);
	
	struct Node *toBeRemoved;
	for (i = 0; i < (total - 1); i++)
	{
		toBeRemoved = removeNode(&holla);
		free(toBeRemoved);

	}

	traverse(&holla);
	//remove the last item
	removeNode(&holla);
	//should be empty:
	traverse(&holla);
}

