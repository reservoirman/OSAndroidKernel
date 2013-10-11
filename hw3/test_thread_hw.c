#define _REENTRANT
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "sthread.h"
#include "sync.h"

sthread_sem_t test;

int
threadmain(void *arg)
{

  sthread_sem_down(&test);
  int threadno = (int)arg;
  for (;;) {
    printf("thread %d: I'm going to sleep\n", threadno);
    sthread_suspend();
    printf("thread %d: I woke up!\n", threadno);
  }
  sthread_sem_up(&test);
  return 0;
}

int
main(int argc, char *argv[])
{
  sthread_t thr1, thr2;
  printf("test retrun val:%d\n", sthread_sem_init(&test, 1));

  if (sthread_init() == -1)
    fprintf(stderr, "%s: sthread_init: %s\n", argv[0], strerror(errno));

  if (sthread_create(&thr1, threadmain, (void *)1) == -1)
    fprintf(stderr, "%s: sthread_create: %s\n", argv[0], strerror(errno));

  if (sthread_create(&thr2, threadmain, (void *)2) == -1)
    fprintf(stderr, "%s: sthread_create: %s\n", argv[0], strerror(errno));

  sleep(1);
  //sthread_wake(thr1);
  sleep(1);
  //sthread_wake(thr2);
  sleep(1);
  //sthread_wake(thr1);
  //sthread_wake(thr2);
  sleep(1);

  return 0;
}
