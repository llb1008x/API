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
#include <linux/miscdevice.h>
#include <linux/proc_fs.h>
#include <asm/atomic.h>

#include <linux/platform_device.h>

#define	DEMO_DEBUG
#ifdef 	DEMO_DEBUG
#define	dem_dbg(fmt, arg...)		printk(KERN_WARNING fmt, ##arg)
#else
#define	dem_dbg(fmt, arg...)		printk(KERN_DEBUG fmt, ##arg)
#endif


static struct kfifo key_fifo;
static spinlock_t key_spinlock;
static struct workqueue_struct *key_workqueue;
static struct work_struct key_work;
static struct timer_list key_timer;
DECLARE_WAIT_QUEUE_HEAD(key_queue);
DECLARE_WAIT_QUEUE_HEAD(select_queue);

//存储中断号
static unsigned int irq;

atomic_t open_flag = ATOMIC_INIT(0);


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

static struct miscdevice key_misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.fops = &fops,
	.name = "demo0",
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

static int key_probe(struct platform_device *pdev)
{
	int retval;
	struct resource *res = NULL;
	unsigned int gpio;
	dem_dbg("[msg]:this is a driver demo, in module initial function\n");

	/*获取platform_device 资源*/
	res = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	if (res == NULL) {
		dem_dbg("[err]: no memory resource specified\n");
		return PTR_ERR(res);
	}

	irq = res->start;

	res = platform_get_resource(pdev, IORESOURCE_IO, 0);
	if (res == NULL) {
		dem_dbg("[err]: no memory resource specified\n");
		return PTR_ERR(res);
	}
	gpio = res->start;

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

	//key_ioport_init();
	s3c_gpio_cfgpin(gpio,S3C_GPIO_OUTPUT);
	gpio_set_value(gpio,0);
	s3c_gpio_setpull(S5PV210_GPH2(3),S3C_GPIO_PULL_UP);
	
	retval = request_irq(irq, key_irqhandler, IRQ_TYPE_EDGE_BOTH, 
			"demo_key", NULL);
	if(retval){
		dem_dbg("[err]:request eint20 failed!\n");
		goto irq_err;
	}

	disable_irq(irq);	//中断关闭

	retval = misc_register(&key_misc);
	if(retval < 0){
		dem_dbg("[err]:register misc device failed!\n");
		goto chrdev_err;
	}

	
	return 0;


chrdev_err:
	free_irq(irq, NULL);
	
irq_err:
	destroy_workqueue(key_workqueue);
	
wq_err:
	kfifo_free(&key_fifo);
	return retval;
}

static int key_remove(struct platform_device *dev)
{
	dem_dbg("[msg]:in module exit function\n");

	del_timer_sync(&key_timer);

	kfifo_free(&key_fifo);
	flush_workqueue(key_workqueue);
	destroy_workqueue(key_workqueue);
	
	free_irq(irq, NULL);

	misc_deregister(&key_misc);

	return 0;
}

static struct platform_driver key_driver = {
		.probe = key_probe,
		.remove = key_remove,
		.driver = {
			.owner = THIS_MODULE,
			.name = "s5pv210-key",
		},
};

static int __init drvdemo_init(void)
{
	return platform_driver_register(&key_driver);
}

static void __exit drvdemo_exit(void)
{
	platform_driver_unregister(&key_driver);
}

module_init(drvdemo_init);
module_exit(drvdemo_exit);


MODULE_LICENSE("Dual BSD/GPL");	//BSD/GPL双重许可证
MODULE_AUTHOR("sunplusedu");		//模块作者(可选)
MODULE_DESCRIPTION("used for studing linux drivers");	//模块儿简介(可选)
