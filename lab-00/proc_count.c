#include <linux/module.h>
#include <linux/printk.h>

static struct proc_dir_entry *count_file;
static int proc_count_show(struct seq_file *m, void *v)
{
	struct task_struct *p;
	int count = 0;
	for_each_process(p)
	{
		count = count + 1;
	}
	
	seq_printf(m, "%d\n", count); 
	return 0;
}


static int __init proc_count_init(void)
{
	pr_info("proc_count: init\n");
	count_file = proc_create_single("count", 0, NULL, proc_count_show);
	
	return 0;
}

static void __exit proc_count_exit(void)
{
	pr_info("proc_count: exit\n");
	proc_remove(count_file);
}

module_init(proc_count_init);
module_exit(proc_count_exit);

MODULE_AUTHOR("Akshay Gupta");
MODULE_DESCRIPTION("This module reports the number of running processes on the machine.");
MODULE_LICENSE("GPL");
