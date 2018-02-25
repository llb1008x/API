/*
* Copyright (c) 2014 sunplusedu
* This is a demo, about hwo to create a module
*
* Date and Dition:	2014-04-12	v1.0
* Author:				<xiuhai.deng@sunplusapp.com>
* 
*/

/*
* debug:
*		./t
* 		cat /dev/demo_dev
*		echo "xxx" > /dev/demo_dev
* 		cat xxx.y > /dev/demo_dev
*/

#include <linux/module.h>	/* module_init */
#include <linux/fs.h>	/* file_operations */
#include <linux/device.h>	/* class device */
#include <linux/sched.h>		/* current */
#include <linux/mount.h>		/* struct vfsmount */
#include <asm/io.h>	/* writel() */
#include <plat/gpio-cfg.h>	/* s3c_gpio_cfgpin */
#include <mach/gpio.h>	/* gpio_set_value */
#include <linux/uaccess.h> /* copy_to_user() */
#include <linux/interrupt.h> /* request_irq() */
#include <linux/irq.h> /* IRQ_TYPE_EDGE_FALLING() */
#include <linux/delay.h> /* mdelay() */
#include <linux/kfifo.h> /* kfifo */
#include <linux/poll.h> /* poll */
#include <linux/kthread.h> /* kthread */
#include <linux/cdev.h> /* cdev_add() */
#include <linux/proc_fs.h> /* proc_dir_entry() */
#include <linux/platform_device.h> /* platform_device_register() */
#include <linux/ioport.h> /* platform_device_register() */
#include <linux/input.h> /* platform_device_register() */

struct kobject *parent_obj;  
struct kobject *child_obj;  

static ssize_t demo_kobj_show(struct kobject *kobj,struct attribute *attr,char *buf)
{
	printk(KERN_WARNING "%s\n",__FUNCTION__);
	return snprintf(buf,PAGE_SIZE,"This is demo kobject\n");
}
static ssize_t demo_kobj_store(struct kobject *kobj,struct attribute *attr,const char *buf,size_t count)
{
	printk(KERN_WARNING "%s:%s\n",__FUNCTION__,buf);
	return sizeof(buf);
}

struct sysfs_ops demo_sysops = {  
	.show = demo_kobj_show,  
	.store = demo_kobj_store,  
};  
static struct kobj_type demo_child_ktype = {
	.sysfs_ops = &demo_sysops,
};
static struct attribute demo_child_att = {
	.name = "demo_sys_att",	
	.mode = S_IRUGO|S_IWUSR,
};

static int __init demo_kobject_init(void)
{
	signed int ret = 0;
	printk(KERN_WARNING "%s\n",__FUNCTION__);

	parent_obj = kobject_create_and_add("parent_obj", NULL);  
	//child_obj = kobject_create_and_add("child_obj", parent_obj);  	
	child_obj = kzalloc(sizeof(*child_obj), GFP_KERNEL);
	if (!child_obj)
		return -1;

	kobject_init(child_obj, &demo_child_ktype);
	ret = kobject_add(child_obj,parent_obj,"child_obj");
	/* 为内核对象创建操作属性文件 */
	ret = sysfs_create_file(child_obj,&demo_child_att);
	
	return 0;  
}
static void __exit demo_kobject_exit(void)
{
	printk(KERN_WARNING "%s\n",__FUNCTION__);
	sysfs_remove_file(child_obj,&demo_child_att);
	kobject_del(child_obj);
	kzfree(child_obj);
	kobject_del(parent_obj); 	
	kzfree(parent_obj);
}

module_init(demo_kobject_init);
module_exit(demo_kobject_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("xiuhai.deng@sunplusapp.com");


