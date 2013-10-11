#define _REENTRANT
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "sthread.h"
#include "sync.h"
#include <stdlib.h>

 	sthread_sem_t test;

/*int
threadmain(void *arg)
{
	int threadno = (int)arg;
	for (;;) 
  	{
		sthread_sem_down(&test);
    		printf("thread %d: I'm going to sleep\n", threadno);
  		sthread_suspend();
    		printf("thread %d: I woke up!\n", threadno);
  	}
  	return 0;
}*/

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
int deposit1(void *arg)
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

int withdraw1(void *arg)
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


int main(int argc, char *argv[])
{

	sthread_t thr1, thr2, thr3, thr4, thr5;

     // Test sthread_sem_init and sthread_sem_destroy 

	test.sthread_count = 0;
	printf("test retrun val:%d\n", sthread_sem_init(&test, 2)); 
 	printf("test init:%d\n", test.count);
     //	printf("test return destroy:%d\n", sthread_sem_destroy(&test));
 

 	if (sthread_init() == -1)
   		fprintf(stderr, "%s: sthread_init: %s\n", argv[0], strerror(errno));

	if (sthread_create(&thr1, withdraw, (void *)1) == -1)
    		fprintf(stderr, "%s: sthread_create: withdraw %s\n", argv[0], strerror(errno));
	if (sthread_create(&thr2, withdraw1, (void *)2) == -1)
    		fprintf(stderr, "%s: sthread_create: withdraw %s\n", argv[0], strerror(errno));	
	if (sthread_create(&thr3, deposit, (void *)3) == -1)
   		fprintf(stderr, "%s: sthread_create: deposit %s\n", argv[0], strerror(errno));
	if (sthread_create(&thr4, deposit1, (void *)4) == -1)
   		fprintf(stderr, "%s: sthread_create: deposit %s\n", argv[0], strerror(errno));

	

	 int i = 0;
	 while (i < 30)
	 {
		
		printf("Balance = %d\n", balance);
		sleep(1);
		i++;
	 }


	/*
  	 printf("down ret:%d\n", sthread_sem_down(&test));
 	 printf("down ret:%d\n", sthread_sem_down(&test));
 	 printf("down ret:%d\n", sthread_sem_down(&test));
 	 printf("down ret:%d\n", sthread_sem_down(&test));
 	 printf("down ret:%d\n", sthread_sem_down(&test)); 


	 sthread_wake(thr1);
     //  printf("All done.  Balance = %d\n", balance);
  	 sthread_wake(thr2);
     //  printf("All done.  Balance = %d\n", balance);
	 while (1)
	 {
		printf("All done.  Balance = %d\n", balance);
		sleep(1);
	 }*/
/*
 if (sthread_create(&thr1, threadmain, (void *)1) == -1)
    fprintf(stderr, "%s: sthread_create: %s\n", argv[0], strerror(errno));


  if (sthread_create(&thr2, threadmain, (void *)2) == -1)
    fprintf(stderr, "%s: sthread_create: %s\n", argv[0], strerror(errno));


if (sthread_create(&thr3, threadmain, (void *)3) == -1)
    fprintf(stderr, "%s: sthread_create: %s\n", argv[0], strerror(errno));

if (sthread_create(&thr4, threadmain, (void *)4) == -1)
    fprintf(stderr, "%s: sthread_create: %s\n", argv[0], strerror(errno));

if (sthread_create(&thr5, threadmain, (void *)5) == -1)
    fprintf(stderr, "%s: sthread_create: %s\n", argv[0], strerror(errno));



sleep(1);
sthread_wake(thr1);
sleep(1);
sthread_wake(thr2);
sleep(1);
printf("%s", "\n\n\n");

sthread_wake(thr1);
sleep(1);
sthread_wake(thr2);
sleep(1);
sthread_wake(thr3);
sleep(1);
sthread_wake(thr4);
sleep(1);
sthread_wake(thr5);
sleep(1);
*/
	//queueTest();

	return 0;
}

