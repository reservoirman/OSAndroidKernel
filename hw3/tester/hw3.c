#include <stdio.h>
#include <sys/utsname.h>
//#include <sys/printk.h>

#define TOTAL_CALLS 20
#include <sys/syscall.h>


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

     enum __netlock_t {
        NET_LOCK_N, /* Placeholder for no lock */
	NET_LOCK_R, /* Indicates regular lock */
	NET_LOCK_E, /* Indicates exclusive lock */
     } ;


int main() {
	/*printf("Return val: %d\n", syscall(378, NET_LOCK_R)); // Obtain lock
        		printf("%d. Regular lock acquired\n",0);
    //   sleep(10);
      printf("Return val: %d\n", syscall(379)); // Release lock
	*/
	int status;
	pid_t child;
	int i;
    for(i = 0; i < 7; i++) {
		if((child = fork()) == 0) {
			// child process
			if (i < 4) { 
				printf("Return val: %d\n", syscall(378, NET_LOCK_R)); // Obtain lock
        		printf("%d. Regular lock acquired\n", i);
        		printf("Return val: %d\n", syscall(379)); // Release lock	
        	} 
			else if (i >= 4 && i < 6)
			{
				printf("Return val: %d\n", syscall(378, NET_LOCK_E)); // Obtain lock
        		printf("%d. Exclusive lock acquired\n", i);
        		printf("Return val: %d\n", syscall(379)); // Release lock	
			}
			else
			{
				printf("Return val: %d\n", syscall(378, NET_LOCK_R)); // Obtain lock
        		printf("%d. Regular lock acquired\n", i);
        		printf("Return val: %d\n", syscall(379)); // Release lock	
			}
			exit(0);
		} 
		else 
		{
			// parent process
			// parent points to child id
			pid_t childPid = wait(&status);
			printf("Done son!\n");
		}

	}
	
   return 0;
}
