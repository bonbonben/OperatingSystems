#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/seq_file.h>
static int proc_show(struct seq_file *m,void *v)	//use for_each_process print pid
{
	struct task_struct *task_list;
	for_each_process(task_list)
	{
		seq_printf(m,"\n %s %d \n",task_list->comm,task_list->pid);
  	}
  	return 0;
}
static int proc_open(struct inode *inode, struct  file *file)	//open file
{
  	return single_open(file, proc_show, NULL);
}
static const struct file_operations proc_fops = 
{
  	.owner = THIS_MODULE,
  	.open = proc_open,
  	.read = seq_read,
  	.llseek = seq_lseek,
  	.release = single_release,
};
static int __init proc_init(void)	//create proc
{
  	proc_create("proc", 0, NULL, &proc_fops);
  	return 0;
}
static void __exit proc_exit(void)	//exit proc
{
	printk(KERN_INFO "goodbye\n"); 	
	remove_proc_entry("proc",NULL);
}
MODULE_LICENSE("GPL");
module_init(proc_init);
module_exit(proc_exit);
