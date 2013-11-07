#include <linux/sched.h>
//#include "mycfs.h"
#include "sched.h"

static void enqueue_task_mycfs(struct rq *rq, struct task_struct *p, int flags)
{
	p->on_rq = 1;
	//inc_nr_running(rq);
}

static void dequeue_task_mycfs(struct rq *rq, struct task_struct *prev,int flags)
{
	prev->on_rq = 0;
	//dec_nr_running(rq);
}

//this will be the big one to implement!!!
static struct task_struct *pick_next_task_mycfs(struct rq *rq)
{
	
	struct task_struct *curr = rq->curr;
	
	if (curr && curr->on_rq)
	{
		printk("pick_next_task_mycfs: Process %d eligible to run!\n", curr->pid);	
		return curr;
	}
	//printk("pick_next_task_mycfs: no processes eligible to be run\n");
	return NULL;
}

static void put_prev_task_mycfs(struct rq *rq, struct task_struct *prev)
{
	//printk("put_prev_task_mycfs\n");
}



static void yield_task_mycfs(struct rq *rq)
{
}

static void check_preempt_curr_mycfs(struct rq *rq, struct task_struct *p, int flags)
{
	//resched_task(rq->curr);
}

static int select_task_rq_mycfs(struct task_struct *p, int sd_flag, int wake_flags)
{

	return task_cpu(p); 
	
}

static void set_curr_task_mycfs(struct rq *rq)
{
}

static unsigned int get_rr_interval_mycfs(struct rq *rq, struct task_struct *task)
{
	return 0;
}

static void
prio_changed_mycfs(struct rq *rq, struct task_struct *p, int oldprio)
{
}

static void task_tick_mycfs(struct rq *rq, struct task_struct *curr, int queued)
{
}

static void task_fork_mycfs(struct task_struct *p)
{
}

void init_mycfs_rq(struct mycfs_rq *mycfs_rq)
{
	//creating the RB tree for MYCFS:
	mycfs_rq->tasks_timeline = RB_ROOT;
	mycfs_rq->min_vruntime = (u64)(-(1LL << 20));
#ifndef CONFIG_64BIT
	mycfs_rq->min_vruntime_copy = mycfs_rq->min_vruntime;
#endif
}

const struct sched_class mycfs_sched_class = {
	.next = &idle_sched_class,
	.dequeue_task	= dequeue_task_mycfs,
	.pick_next_task = pick_next_task_mycfs,
	.enqueue_task = enqueue_task_mycfs,
	.put_prev_task	= put_prev_task_mycfs,
	.yield_task		= yield_task_mycfs,
	.check_preempt_curr	= check_preempt_curr_mycfs,
	#ifdef CONFIG_SMP
	.select_task_rq		= select_task_rq_mycfs,
	#endif
	.set_curr_task		= set_curr_task_mycfs,
	.get_rr_interval	= get_rr_interval_mycfs,
	.prio_changed		= prio_changed_mycfs,
	.task_tick		= task_tick_mycfs,
	.task_fork		= task_fork_mycfs,
};

