/*
 * Copyright  2011-2012  sunplusapp
 * driver demo for sunplusedu
 */

/*
 * this program is free software; you can redistribute it and/or modify
 * it
 * 
 * Date and Edition:		2012-12-27  v1.3
 * Date and Edition:		2013-12-02  v2.0
 * Author:				"driving team" of sunplusedu
 */

#include <linux/init.h>	
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <asm/io.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <mach/gpio.h>
#include <plat/gpio-cfg.h>
#include <linux/workqueue.h>
#include <linux/kfifo.h>
/*原子变量对应的头文件*/
#include <asm/atomic.h>

#define	DEMO_DEBUG
#ifdef 	DEMO_DEBUG
#define	dem_dbg(fmt, arg...)		printk(KERN_WARNING fmt, ##arg)
#else
#define	dem_dbg(fmt, arg...)		printk(KERN_DEBUG fmt, ##arg)
#endif

#define	DEVICE_COUNT			1

static struct kfifo key_fifo;

static struct workqueue_struct *key_workqueue;
static struct work_struct key_work;

//open引用计数，没打开一次计数值加一，关闭则相反
static int open_flag = 0;

static int major = 0;
static int demo_open (struct inode *pnode, struct file *filp)
{
	dem_dbg("[kern func]: %s  major: %d  minor: %d\n",
					__FUNCTION__, imajor(pnode), iminor(pnode));
	if(!open_flag)
		enable_irq(IRQ_EINT(19));	//打开中断
	open_flag++;
	
	return 0;
}

static ssize_t demo_read (struct file *filp, char __user *buf, size_t count, loff_t *offp)
{
	int key;
	unsigned long len = min(count, sizeof(key));
	int retval;

	/*<步骤三>: 从kfifo中读取数据,如果读取失败key值赋-1*/
	retval = kfifo_out(&key_fifo, &key, sizeof(key));
	if(retval != sizeof(key)){
		dem_dbg("[warning]: the kfifo has no data!\n");
		key = -1;
	}

	if(copy_to_user(buf, &key, len) != 0){
		retval = -EFAULT;
		goto cp_err;
	}
	
	return len;
cp_err:
	return retval;	
}

static int demo_release (struct inode *pnode, struct file *filp)
{
	dem_dbg("[kern func]: %s  major: %d  minor: %d\n",
					__FUNCTION__, imajor(pnode), iminor(pnode));
	open_flag--;
	if(!open_flag)
		disable_irq(IRQ_EINT(19));		//关闭中断
	
	return 0;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = demo_read,
	.open = demo_open,
	.release = demo_release,
};	

static void key_workhandler(struct work_struct *work)
{
	int key = 1;
	int retval;
	
	dem_dbg("[kern func]: %s  do work handler\n", __FUNCTION__);

	/*<步骤三>: 添加数据到kfifo中*/
	retval = kfifo_in(&key_fifo, &key, sizeof(key));
	if(retval != sizeof(key))
		dem_dbg("[warning]: the kfifo has no enough space!\n");
}

irqreturn_t key_irqhandler(int irq, void *dev_id)
{
	dem_dbg("[kern func]: %s  in irq handler\n", __FUNCTION__);
	
	queue_work(key_workqueue, &key_work);

	return IRQ_HANDLED;
}

static void key_ioport_init(void)
{
	s3c_gpio_cfgpin(S5PV210_GPH3(0), S3C_GPIO_OUTPUT);
	s3c_gpio_cfgpin(S5PV210_GPH3(1), S3C_GPIO_OUTPUT);
	gpio_set_value(S5PV210_GPH3(0), 0);
	gpio_set_value(S5PV210_GPH3(1), 0);

	s3c_gpio_setpull(S5PV210_GPH2(3), S3C_GPIO_PULL_UP);
}

static struct class *demo_class;
static int __init drvdemo_init(void)
{
	struct device *demo_device;
	int i;
	int retval;
	
	dem_dbg("[msg]:this is a driver demo, in module initial function\n");

	/*<步骤二>: 申请fifo空间128字节
	  *   GFP_KERNEL表示没有足够的内存空间时，睡眠等待，
	  *   与之相对的是GFP_ATOMIC，没有足够的空间时不睡眠
	  */
	if(kfifo_alloc(&key_fifo, 128, GFP_KERNEL) != 0){
		dem_dbg("[err]: kfifo malloc failed!\n");
		return -ENOMEM;
	}
	
	key_workqueue = create_workqueue("key_workqueue");
	if(IS_ERR(key_workqueue)){
		dem_dbg("[err]:create workqueue failed!\n");
		retval = PTR_ERR(key_workqueue);
		goto wq_err;
	}
	INIT_WORK(&key_work, key_workhandler);

	key_ioport_init();
	retval = request_irq(IRQ_EINT(19), key_irqhandler, IRQ_TYPE_EDGE_BOTH, 
			"demo_key", NULL);
	if(retval){
		dem_dbg("[err]:request eint20 failed!\n");
		goto irq_err;
	}

	disable_irq(IRQ_EINT(19));	//中断关闭使能

	major = register_chrdev(0, "demo_chrdev", &fops);
	if(major < 0){
		retval = major;
		goto chrdev_err;
	}

	demo_class = class_create(THIS_MODULE,"demo_class");
	if(IS_ERR(demo_class)){
		retval =  PTR_ERR(demo_class);
		goto class_err;
	}

	for(i=0; i<DEVICE_COUNT; i++){
		demo_device = device_create(demo_class,NULL, MKDEV(major, i), NULL,"demo%d",i);
		if(IS_ERR(demo_device)){
			retval = PTR_ERR(demo_device);
			goto device_err;
		}
	}
	return 0;
	
device_err:
	while(i--)
		device_destroy(demo_class,MKDEV(major, i));
	class_destroy(demo_class);
	
class_err:
	unregister_chrdev(major, "demo_chrdev");
	
chrdev_err:
	free_irq(IRQ_EINT(19), NULL);
	
irq_err:
	destroy_workqueue(key_workqueue);
wq_err:
	kfifo_free(&key_fifo);
	return retval;
}

static void __exit drvdemo_exit(void)
{
	int i;
	
	dem_dbg("[msg]:in module exit function\n");

	/*<步骤四>: 释放fifo空间128字节*/
	kfifo_free(&key_fifo);

	flush_workqueue(key_workqueue);
	destroy_workqueue(key_workqueue);
	
	free_irq(IRQ_EINT(19), NULL);
	unregister_chrdev(major, "demo_chrdev");
	for(i=0; i<DEVICE_COUNT; i++)
		device_destroy(demo_class,MKDEV(major, i));
	class_destroy(demo_class);
}

module_init(drvdemo_init);
module_exit(drvdemo_exit);


MODULE_LICENSE("Dual BSD/GPL");	//BSD/GPL双重许可证
MODULE_AUTHOR("sunplusedu");		//模块作者(可选)
MODULE_DESCRIPTION("used for studing linux drivers");	//模块儿简介(可选)
