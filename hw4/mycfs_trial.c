#include <linux/sched.h>
//#include "mycfs.h"
#include "sched.h"

struct sched_mycfs_entity *__pick_first_mycfs_entity(struct mycfs_rq *mycfs_rq)
{
	struct rb_node *left = mycfs_rq->rb_leftmost;

	if (!left)
		return NULL;

	return rb_entry(left, struct sched_mycfs_entity, run_node);
}

static struct sched_mycfs_entity *pick_next_entity(struct mycfs_rq *mycfs_rq)
{
	struct sched_mycfs_entity *se= __pick_first_mycfs_entity(mycfs_rq);
	return se;
} 


static void __dequeue_entity(struct mycfs_rq *mycfs_rq, struct sched_entity *se)
{
	if (mycfs_rq->rb_leftmost == &se->run_node) 
	{
		struct rb_node *next_node;

		next_node = rb_next(&se->run_node);
		mycfs_rq->rb_leftmost = next_node;
	}

	rb_erase(&se->run_node, &mycfs_rq->tasks_timeline);
}
static inline void update_stats_curr_start(struct mycfs_rq *mycfs_rq, struct sched_entity *se)
{
	/*
	 * We are starting a new run period:
	 */
	se->exec_start =mycfs_rq->rq->clock_task;
} 

static void set_next_entity(struct mycfs_rq *mycfs_rq, struct sched_entity *se)
{
	/* 'current' is not kept within the tree. */
	if (se->on_rq)
	{
		__dequeue_entity(mycfs_rq, se);
	}

	update_stats_curr_start(mycfs_rq, se);
	mycfs_rq->curr = se;
}




static void dequeue_task_mycfs(struct rq *rq, struct task_struct *prev,int flags)
{

}

static void enqueue_task_mycfs(struct rq *rq, struct task_struct *p, int flags)
{

}
//this will be the big one to implement!!!
static struct task_struct *pick_next_task_mycfs(struct rq *rq)
{
	struct task_struct *p;
	struct mycfs_rq *mycfs_rq = &rq->mycfs;
	struct sched_entity *se;

	if (!mycfs_rq->nr_running)
		return NULL;

	do {
		se = pick_next_entity(mycfs_rq);
		set_next_entity(mycfs_rq, se);
	} while (mycfs_rq);

	p = task_of(se);
	/*if (hrtick_enabled(rq))
		hrtick_start_fair(rq, p);
	*/	

	return p;
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
	return 0;
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




