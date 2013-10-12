#include <stdio.h>
#include <sys/utsname.h>
#include "inject.h"
//#include <sys/printk.h>

#define TOTAL_CALLS 20
#include <sys/syscall.h>

     enum __netlock_t {
        NET_LOCK_N, /* Placeholder for no lock */
	NET_LOCK_R, /* Indicates regular lock */
	NET_LOCK_E, /* Indicates exclusive lock */
     } ;




int main() {
	int ret = syscall(378, NET_LOCK_E);
	printf("return of acquire: %d", ret);
		
   return 0;
}
