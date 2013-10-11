#ifndef _NETLOCK_H
#define _NETLOCK_H
/* Syscall 378. Acquire netlock. type indicates
        whether a regular or exclusive lock is needed. Returns 0 on success 
	and -1 on failure.  
      */
     
     enum __netlock_t {
        NET_LOCK_N, /* Placeholder for no lock */
	NET_LOCK_R, /* Indicates regular lock */
	NET_LOCK_E, /* Indicates exclusive lock */
     } ;
     typedef enum __netlock_t netlock_t;
	
      int netlock_acquire(netlock_t type);

     /* Syscall 379. Release netlock. Return 0 on success and -1 on failure.  
      */
     int netlock_release(void);


#endif
