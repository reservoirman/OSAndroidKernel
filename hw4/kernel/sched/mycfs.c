#include <linux/sched.h>
##include "mycfs.h"
#include "sched.h"

static void dequeue_task_mycfs(struct rq *rq, struct task_struct *prev,int flags)
{

}

static void enqueue_task_mycfs(struct rq *rq, struct task_struct *p, int flags)
{

}
//this will be the big one to implement!!!
static struct task_struct *pick_next_task_mycfs(struct rq *rq)
{
	printk("myCFS scheduler is called\n");
	return 0;
}

static void put_prev_task_mycfs(struct rq *rq, struct task_struct *prev)
{

}

const struct sched_class mycfs_sched_class = {
	.next = &idle_sched_class,
	.dequeue_task	= dequeue_task_mycfs,
	.pick_next_task = pick_next_task_mycfs,
	.enqueue_task = enqueue_task_mycfs,
	.put_prev_task	= put_prev_task_mycfs,
	//the other hooks will need to be implemented too	
};




