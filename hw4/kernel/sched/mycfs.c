#include <linux/sched.h>
//#include "mycfs.h"
#include "sched.h"





static void dequeue_task_mycfs(struct rq *rq, struct task_struct *prev,int flags)
{
	dec_nr_running(rq);
}

static void enqueue_task_mycfs(struct rq *rq, struct task_struct *p, int flags)
{
	inc_nr_running(rq);

}
//this will be the big one to implement!!!
static struct task_struct *pick_next_task_mycfs(struct rq *rq)
{
	struct task_struct *stop = rq->stop;

	if (stop && stop->on_rq)
		return stop;


	return NULL;
}






static void put_prev_task_mycfs(struct rq *rq, struct task_struct *prev)
{
	//printk("put_prev_task_mycfs\n");
}

static void yield_task_mycfs(struct rq *rq)
{
}

static int select_task_rq_mycfs(struct task_struct *p, int sd_flag, int wake_flags)
{

	return task_cpu(p); 
	
}

static void task_tick_mycfs(struct rq *rq, struct task_struct *curr, int queued)
{
}

static void task_fork_mycfs(struct task_struct *p)
{
}

const struct sched_class mycfs_sched_class = {
	.next = &idle_sched_class,
	.dequeue_task	= dequeue_task_mycfs,
	.pick_next_task = pick_next_task_mycfs,
	.enqueue_task = enqueue_task_mycfs,
	.put_prev_task	= put_prev_task_mycfs,
	.yield_task		= yield_task_mycfs,
	#ifdef CONFIG_SMP
	.select_task_rq		= select_task_rq_mycfs,
	#endif
	.task_tick		= task_tick_mycfs,
	.task_fork		= task_fork_mycfs,
};




