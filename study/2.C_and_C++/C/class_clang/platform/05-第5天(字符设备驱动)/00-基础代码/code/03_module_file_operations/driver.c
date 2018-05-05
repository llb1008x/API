/*
* Copyright (c) 2014 sunplusedu
* This is a demo, about hwo to create a module
*
* Date and Dition:	2014-03-12	v1.0
* Author:				<xiuhai.deng@sunplusapp.com>
* 
*/

#include <linux/module.h>	/* module_init */
#include <linux/fs.h>	/* file_operations */


int demo_open(struct inode *pinode, struct file *pfile)
{
	return 0;
}

struct file_operations demo_fops = {
.owner = THIS_MODULE,
.open = demo_open,
};

static int __init demo_module_init(void)
{
	struct module *mod;
	printk(KERN_WARNING "demo_module_init!\n");
	printk(KERN_WARNING "%s\n",THIS_MODULE->name);
	list_for_each_entry(mod, *(&THIS_MODULE->list.prev), list )
		printk(KERN_WARNING "%s\n",mod->name);
	THIS_MODULE->exit();
	
	return 0;
}
static void __exit demo_module_exit(void)
{
	printk(KERN_WARNING "demo_module_exit!\n");	
}

module_init(demo_module_init);
module_exit(demo_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("xiuhai.deng@sunplusapp.com");

