#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/utsname.h>
//#include <sys/printk.h>

#define TOTAL_CALLS 20
#include <sys/syscall.h>




#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>

enum __netlock_t {
        NET_LOCK_N, /* Placeholder for no lock */
	NET_LOCK_R, /* Indicates regular lock */
	NET_LOCK_E, /* Indicates exclusive lock */
     } ;

int balance = 0;

void *reader(void *arg)
{
	printf("Return val: %d\n", syscall(378, NET_LOCK_R)); // Obtain lock
    printf("%d. Regular lock acquired\n", (int)arg);
    printf("Return val: %d\n", syscall(379)); // Release lock	

}

void *writer(void *arg)
{
	int i;
	int balance = 0;
	printf("Return val: %d\n", syscall(378, NET_LOCK_E)); // Obtain lock
    printf("%d. Exclusive lock acquired\n", (int)arg);
    
    printf("Return val: %d\n", syscall(379)); // Release lock	

}


int main(int argc, char *argv[])
{

	pthread_t thr1, thr2, thr3, thr4, thr5;

     // Test sthread_sem_init and sthread_sem_destroy 

     //	printf("test return destroy:%d\n", sthread_sem_destroy(&test));
 


	//if (sthread_create(&thr1, withdraw, (void *)1) == -1)
    		//fprintf(stderr, "%s: sthread_create: withdraw %s\n", argv[0], strerror(errno));
	//if (sthread_create(&thr2,  withdraw, (void *)2) == -1)
    		//fprintf(stderr, "%s: sthread_create: withdraw %s\n", argv[0], strerror(errno));	
	//if (sthread_create(&thr3,  deposit, (void *)3) == -1)
   		//fprintf(stderr, "%s: sthread_create: deposit %s\n", argv[0], strerror(errno));
	if (pthread_create(&thr1,  NULL, reader, (void *)1) == -1)
   		fprintf(stderr, "%s: sthread_create: deposit %s\n", argv[0], strerror(errno));
	if (pthread_create(&thr2,  NULL, reader, (void *)2) == -1)
   		fprintf(stderr, "%s: sthread_create: deposit %s\n", argv[0], strerror(errno));
	if (pthread_create(&thr3,  NULL, reader, (void *)3) == -1)
   		fprintf(stderr, "%s: sthread_create: deposit %s\n", argv[0], strerror(errno));
	if (pthread_create(&thr4,  NULL, writer, (void *)4) == -1)
   		fprintf(stderr, "%s: sthread_create: deposit %s\n", argv[0], strerror(errno));
	if (pthread_create(&thr5,  NULL, writer, (void *)5) == -1)
   		fprintf(stderr, "%s: sthread_create: deposit %s\n", argv[0], strerror(errno));

	//if (sthread_create(&thr5,  deposit, (void *)5) == -1)
   		//fprintf(stderr, "%s: sthread_create: deposit %s\n", argv[0], strerror(errno));

   	int i = 0;
   	while(i < 10)
   	{
   		sleep(1);
   		i++;
   	}

/*
   	pthread_join(thr1,NULL);
   	pthread_join(thr2,NULL);
   	pthread_join(thr3,NULL);
   	pthread_join(thr4,NULL);
   	pthread_join(thr5,NULL);
*/
	return 0;
}
