#define _REENTRANT
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "sthread.h"
//#include "sync_ad.h"
#include "sync.h"
#include <stdlib.h>

sthread_sem_t test;

int balance = 0;

int deposit(void *arg)
{
	int i = 0;
	//while (1)
	{
		sthread_sem_down(&test);
		for (i = 0; i < 1000000000; i++)
		balance++;
		sthread_sem_up(&test);
	}
	return 0;
}

int withdraw(void *arg)
{ 
	int i = 0;
	//while (1)
	{
		sthread_sem_down(&test);
		for (i = 0; i < 1000000000; i++)
		balance--;
		sthread_sem_up(&test);
	}
	return 0;
}
int thread_funk(void *arg)
{	
	
	sthread_sem_down(&test);
	int i=0;
	while( i<5 )	
	{		
	printf("thread %d\n",(int)arg);
	i++;
	sleep(1);
	}
	sthread_sem_up(&test);
}
int thread_funk2(void *arg)
{
	while(sthread_sem_try_down(&test)!=0)
	{	
	sleep(1);
	printf("semaphore still not available\n");
	}	
	printf("finally .... thread %d\n",(int)arg);
	sthread_sem_up(&test);
}

int main(int argc, char *argv[])
{

	sthread_t thr1, thr2, thr3, thr4, thr5;

     // Test sthread_sem_init and sthread_sem_destroy 

	test.sthread_count = 0;
	printf("test return val:%d\n", sthread_sem_init(&test,1)); 
 	printf("test init:%d\n", test.count);
     //	printf("test return destroy:%d\n", sthread_sem_destroy(&test));
 

 	if (sthread_init() == -1)
   		fprintf(stderr, "%s: sthread_init: %s\n", argv[0], strerror(errno));

	//if (sthread_create(&thr1, withdraw, (void *)1) == -1)
    		//fprintf(stderr, "%s: sthread_create: withdraw %s\n", argv[0], strerror(errno));
	//if (sthread_create(&thr2,  withdraw, (void *)2) == -1)
    		//fprintf(stderr, "%s: sthread_create: withdraw %s\n", argv[0], strerror(errno));	
	//if (sthread_create(&thr3,  deposit, (void *)3) == -1)
   		//fprintf(stderr, "%s: sthread_create: deposit %s\n", argv[0], strerror(errno));
	if (sthread_create(&thr1,  thread_funk2, (void *)1) == -1)
   		fprintf(stderr, "%s: sthread_create: deposit %s\n", argv[0], strerror(errno));
	if (sthread_create(&thr2,  thread_funk, (void *)2) == -1)
   		fprintf(stderr, "%s: sthread_create: deposit %s\n", argv[0], strerror(errno));
	if (sthread_create(&thr3,  thread_funk, (void *)3) == -1)
   		fprintf(stderr, "%s: sthread_create: deposit %s\n", argv[0], strerror(errno));
	if (sthread_create(&thr4,  thread_funk, (void *)4) == -1)
   		fprintf(stderr, "%s: sthread_create: deposit %s\n", argv[0], strerror(errno));
	if (sthread_create(&thr5,  thread_funk, (void *)5) == -1)
   		fprintf(stderr, "%s: sthread_create: deposit %s\n", argv[0], strerror(errno));

	//if (sthread_create(&thr5,  deposit, (void *)5) == -1)
   		//fprintf(stderr, "%s: sthread_create: deposit %s\n", argv[0], strerror(errno));

	int i=0;
	while(i < 30)
	{
	//printf("balance: %d\n", balance);
	
	sleep(1);
	i++;	
	}
	return 0;
}


