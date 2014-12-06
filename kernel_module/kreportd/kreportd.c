#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kthread.h>
#include <linux/err.h>

#define MODULE_VERS "1.0"
#define MODULE_NAME "kreportd"

static struct task_struct *report_task;

static int do_something(void *arg)
{
	int num_task = 0;
	int num_task_run = 0, num_task_int = 0,
		num_task_unint = 0, num_task_nonint = 0, 
		num_task_stop = 0, num_task_trac = 0,
		num_exit_dead = 0, num_exit_zomb = 0;
		

	struct task_struct *p;
	long state;
	char *str_state;

	for(p = &init_task; !kthread_should_stop() && (p = next_task(p)) != &init_task; ){
		num_task++;
		state = p->state;
		switch(state){
		case TASK_RUNNING:
			num_task_run++;
			str_state = "RUNNING";
			break;
		case TASK_INTERRUPTIBLE:
			num_task_int++;
			str_state = "INTERRUPTIBLE";
			break;
		case TASK_UNINTERRUPTIBLE:
			num_task_unint++;
			str_state = "UNINTERRUPTIBLE";
			break;
		case TASK_STOPPED:
			num_task_stop++;
			str_state = "STOPPED";
			break;
		case TASK_TRACED:
			num_task_trac++;
			str_state = "TRACED";
			break;
		case EXIT_ZOMBIE:
			num_exit_zomb++;
			str_state = "ZOMBLE";
			break;
		case EXIT_DEAD:
			num_exit_dead++;
			str_state = "DEAD";
			break;
		/*case TASK_NONINTERACTIVE:
			num_task_nonint++;
			str_state = "TASK_NONINTERACTIVE";
			break;
		*/
		}
		printk("[pid]%6d [pname]%18s [parent-pname]%18s [psatet]%15s \n",p->pid, p->comm, p->parent->comm, str_state );
	}

	printk(KERN_INFO "Number of total process: %d\n", num_task);
	printk(KERN_INFO "Number of running process: %d\n", num_task_run);
	printk(KERN_INFO "Number of interruptible process: %d\n", num_task_int);
	printk(KERN_INFO "Number of uninterruptible process: %d\n", num_task_unint);
	printk(KERN_INFO "Number of stopped process: %d\n", num_task_stop);
	printk(KERN_INFO "Number of traced process: %d\n", num_task_trac);
	printk(KERN_INFO "Number of zombie process: %d\n", num_exit_zomb);
	printk(KERN_INFO "Number of dead process: %d\n", num_exit_dead);

	return 0;
}



static int __init init_kreportd(void)
{
	printk(KERN_INFO "[enter] init_kreportd\n");

	report_task = kthread_run(do_something, NULL, "kreportd");
	printk(KERN_INFO "the pointer is %x", (unsigned int)report_task);

	if(IS_ERR(report_task)){
		printk(KERN_ERR "kreportd create failed.\n");
	}
	else
		printk(KERN_INFO "%s %s create seccesful.\n", MODULE_NAME, MODULE_VERS);

	return 0;
}

static void __exit cleanup_kreportd(void)
{
	if(!IS_ERR(report_task)){
		//kthread_stop(report_task);
		printk(KERN_INFO "module removec.\n");
	}
}

module_init(init_kreportd);
module_exit(cleanup_kreportd);

MODULE_LICENSE("GPL");
