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
#include <linux/timer.h>
#include <linux/delay.h>
#include <linux/wait.h>
#include <linux/poll.h>
#include <asm/atomic.h>

#include <linux/cdev.h>

#define	DEMO_DEBUG
#ifdef 	DEMO_DEBUG
#define	dem_dbg(fmt, arg...)		printk(KERN_WARNING fmt, ##arg)
#else
#define	dem_dbg(fmt, arg...)		printk(KERN_DEBUG fmt, ##arg)
#endif

#define	DEVICE_COUNT			1

static struct kfifo key_fifo;
static spinlock_t key_spinlock;
static struct workqueue_struct *key_workqueue;
static struct work_struct key_work;
static struct timer_list key_timer;
DECLARE_WAIT_QUEUE_HEAD(key_queue);
DECLARE_WAIT_QUEUE_HEAD(select_queue);

atomic_t open_flag = ATOMIC_INIT(0);

//static int major = 0;

/*<步骤一>: 定义cdev结构体变量和dev_nr主从设备号变量*/
static struct cdev key_cdev;
static dev_t dev_nr;

static int demo_open (struct inode *pnode, struct file *filp)
{
	dem_dbg("[kern func]: %s  major: %d  minor: %d\n",
					__FUNCTION__, imajor(pnode), iminor(pnode));
	
	if(atomic_read(&open_flag) == 0)
		enable_irq(IRQ_EINT(19));		//打开中断
	atomic_inc(&open_flag);
	
	return 0;
}

static ssize_t demo_read (struct file *filp, char __user *buf, size_t count, loff_t *offp)
{
	int key;
	unsigned long len = min(count, sizeof(key));
	int retval;

	if(!(filp->f_flags & O_NONBLOCK))
		wait_event_interruptible(key_queue, !kfifo_is_empty(&key_fifo));


	spin_lock(&key_spinlock);	
	retval = kfifo_out(&key_fifo, &key, sizeof(key));
	spin_unlock(&key_spinlock);
	if(retval != sizeof(key))
		key = -1;

	if(copy_to_user(buf, &key, len) != 0){
		retval = -EFAULT;
		goto cp_err;
	}
	
	return len;
cp_err:
	return retval;	
}

static unsigned int demo_poll (struct file *pfile, struct poll_table_struct *ptable)
{
	poll_wait(pfile, &select_queue, ptable);
	
	return kfifo_is_empty(&key_fifo) ? 0 : POLLIN | POLLRDNORM;
}

static int demo_release (struct inode *pnode, struct file *filp)
{
	dem_dbg("[kern func]: %s  major: %d  minor: %d\n",
					__FUNCTION__, imajor(pnode), iminor(pnode));

	atomic_dec(&open_flag);
	if(atomic_read(&open_flag) == 0)
		disable_irq(IRQ_EINT(19));	
	
	return 0;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = demo_read,
	.poll = demo_poll,
	.open = demo_open,
	.release = demo_release,
};	



static void key_timerhandler(unsigned long dat)
{
	int key = 1;
	int retval;

	dem_dbg("[kern func]: %s  in timer handler\n", __FUNCTION__);
	
	spin_lock(&key_spinlock);	
	retval = kfifo_in(&key_fifo, &key, sizeof(key));
	spin_unlock(&key_spinlock);
	if(retval != sizeof(key))
		dem_dbg("[warning]: the kfifo has no enough space!\n");
	else{	
		wake_up_interruptible(&key_queue);
		wake_up_interruptible(&select_queue);
	}
	
	mod_timer(&key_timer, jiffies+40);	
}

static void key_workhandler(struct work_struct *work)
{
	int key = 1;
	int retval;
	
	dem_dbg("[kern func]: %s  do work handler\n", __FUNCTION__);

	msleep(50);

	if(gpio_get_value(S5PV210_GPH2(3)) == 0){		//引脚dat寄存器对应位为0，表示按下事件

		spin_lock(&key_spinlock);	
		retval = kfifo_in(&key_fifo, &key, sizeof(key));
		spin_unlock(&key_spinlock);
		if(retval != sizeof(key))
			dem_dbg("[warning]: the kfifo has no enough space!\n");
		else	{
			wake_up_interruptible(&key_queue);
			wake_up_interruptible(&select_queue);
		}
		
		if(!timer_pending(&key_timer)){
			key_timer.expires = jiffies + 1*HZ;
			add_timer(&key_timer);
		}
	}else{	//dat寄存器对应位为1，表示抬起事件
		del_timer_sync(&key_timer);
	}

	
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

	spin_lock_init(&key_spinlock);

	if(kfifo_alloc(&key_fifo, 128, GFP_KERNEL) != 0){
		dem_dbg("[err]: kfifo malloc failed!\n");
		return -ENOMEM;
	}

	init_timer(&key_timer);
	key_timer.data = 0L;
	key_timer.function = key_timerhandler;
	
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

	disable_irq(IRQ_EINT(19));	//中断关闭

/*   简单版驱动注册函数，用下面推荐注册方式进行替换
	major = register_chrdev(0, "demo_chrdev", &fops);
	if(major < 0){
		retval = major;
		goto chrdev_err;
	}
*/
	/*<步骤二>: 动态申请主从设备号*/
	retval = alloc_chrdev_region(&dev_nr,0,DEVICE_COUNT,"demo_chrdev");
	if(retval){
		dem_dbg("[err]: alloc chrdev region failed!\n");
		goto chrdev_err;
	} 

	/*<步骤三>: 初始化cdev数据*/
	cdev_init(&key_cdev, &fops);
	
	/*<步骤四>: 添加cdev变量到内核，完成驱动注册*/
	retval = cdev_add(&key_cdev, dev_nr, DEVICE_COUNT);
	if(retval){
		dem_dbg("[err]: cdev add failed!\n");
		goto cdev_err;
	}


	demo_class = class_create(THIS_MODULE,"demo_class");
	if(IS_ERR(demo_class)){
		retval =  PTR_ERR(demo_class);
		goto class_err;
	}

	for(i=0; i<DEVICE_COUNT; i++){
		demo_device = device_create(demo_class,NULL, MKDEV(MAJOR(dev_nr), i), NULL,"demo%d",i);
		if(IS_ERR(demo_device)){
			retval = PTR_ERR(demo_device);
			goto device_err;
		}
	}
	return 0;

device_err:
	while(i--)
		device_destroy(demo_class,MKDEV(MAJOR(dev_nr), i));
	class_destroy(demo_class);
	
class_err:
	//unregister_chrdev(major, "demo_chrdev");
	cdev_del(&key_cdev);	

cdev_err:
	unregister_chrdev_region(dev_nr, DEVICE_COUNT);

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
	del_timer_sync(&key_timer);

	kfifo_free(&key_fifo);
	flush_workqueue(key_workqueue);
	destroy_workqueue(key_workqueue);
	
	free_irq(IRQ_EINT(19), NULL);

	//unregister_chrdev(major, "demo_chrdev");
	/*<步骤五>: 删除添加的cdev结构体，并释放申请的主从设备号*/
	cdev_del(&key_cdev);	
	unregister_chrdev_region(dev_nr, DEVICE_COUNT);
	
	for(i=0; i<DEVICE_COUNT; i++)
		device_destroy(demo_class,MKDEV(MAJOR(dev_nr), i));
	class_destroy(demo_class);
}

module_init(drvdemo_init);
module_exit(drvdemo_exit);


MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("sunplusedu");
MODULE_DESCRIPTION("used for studing linux drivers");	
