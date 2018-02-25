/*
* Copyright (c) 2014 sunplusedu
* This is a demo, about hwo to create a module
*
* Date and Dition:	2014-03-12	v1.0
* Author:				<xiuhai.deng@sunplusapp.com>
* 
*/

/*
* debug:
* 		cat xxx > /dev/demo_dev
* 		cat /dev/demo_dev
*/

#include <linux/module.h>	/* module_init */
#include <linux/fs.h>	/* file_operations */

#define DEVICE_NAME "demo_char"

static int major = 0;

static int demo_open(struct inode *pinode, struct file *pfile)
{
	printk(KERN_WARNING "%s,%d,%d\n",__FUNCTION__,imajor(pinode),iminor(pinode));
	printk(KERN_WARNING "%s\n",THIS_MODULE->name);
	return 0;
}
static int demo_release(struct inode *pinode, struct file *pfile)
{
	printk(KERN_WARNING "%s,%d,%d\n",__FUNCTION__,imajor(pinode),iminor(pinode));
	return 0;
}

struct file_operations demo_fops = {
.owner = THIS_MODULE,
.open = demo_open,
.release = demo_release,
};

static int __init demo_module_init(void)
{
	int ret = 0;
	
	printk(KERN_WARNING "%s\n",__FUNCTION__);
	major = register_chrdev(major,DEVICE_NAME,&demo_fops);
	if(major < 0)
	{
		ret = major;
		goto register_chrdev_fail;
	}
	return 0;
	
register_chrdev_fail:
	return ret;
}
static void __exit demo_module_exit(void)
{
	printk(KERN_WARNING "%s\n",__FUNCTION__);
	unregister_chrdev(major,DEVICE_NAME);
}

module_init(demo_module_init);
module_exit(demo_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("xiuhai.deng@sunplusapp.com");

