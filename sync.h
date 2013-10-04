/*
 * sync.h
 */

#ifndef _STHREAD_SYNC_H_
#define _STHREAD_SYNC_H_

/*
 * Semaphore structure
 */
typedef struct sthread_sem_struct sthread_sem_t;

struct Node {
    sthread_t data; // sthread
    struct Node *next; // pointer to the next sthread
};

struct sthread_sem_struct {
    int count;
    int sthread_count;
    struct Node *head;
    struct Node *start;
};




int sthread_sem_init(sthread_sem_t *sem, int count);
int sthread_sem_destroy(sthread_sem_t *sem);
int sthread_sem_down(sthread_sem_t *sem);
int sthread_sem_try_down(sthread_sem_t *sem);
int sthread_sem_up(sthread_sem_t *sem);

#endif
