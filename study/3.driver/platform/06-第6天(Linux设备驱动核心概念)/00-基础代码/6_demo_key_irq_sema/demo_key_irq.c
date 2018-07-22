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
#include <asm/atomic.h>
/*�ź�����Ӧͷ�ļ�*/
#include <linux/semaphore.h>

#define	DEMO_DEBUG
#ifdef 	DEMO_DEBUG
#define	dem_dbg(fmt, arg...)		printk(KERN_WARNING fmt, ##arg)
#else
#define	dem_dbg(fmt, arg...)		printk(KERN_DEBUG fmt, ##arg)
#endif

#define	DEVICE_COUNT			1

/*key_fifoΪȫ�ֱ���������ͨ��alloc�����ڶ���Ϊkey_fifo����128�ֽڿռ䣬
  *�ÿռ����ڹ�����Դ��Ҳ�����κ�ʹ�ø������Ľ����Լ��жϷ�����
  *���Է��ʸ���Դ���������в��������Ŀ��ܣ�������̬
  */
static struct kfifo key_fifo;

/*<����һ>: �����ź���*/
static struct semaphore key_sem;

static struct workqueue_struct *key_workqueue;
static struct work_struct key_work;

atomic_t open_flag = ATOMIC_INIT(0);

static int major = 0;
static int demo_open (struct inode *pnode, struct file *filp)
{
	dem_dbg("[kern func]: %s  major: %d  minor: %d\n",
					__FUNCTION__, imajor(pnode), iminor(pnode));

	if(atomic_read(&open_flag) == 0)
		enable_irq(IRQ_EINT(19));		//���ж�
	atomic_inc(&open_flag);
	
	return 0;
}

static ssize_t demo_read (struct file *filp, char __user *buf, size_t count, loff_t *offp)
{
	int key;
	unsigned long len = min(count, sizeof(key));
	int retval;

	/*<������>: �ź�����ʹ��
	  *дfifoǰ�������ź�����1(Ϊ0)
	  *д��fifo��������ź�����1(Ϊ1)
	  */
	down(&key_sem);		//�ź�����1
	retval = kfifo_out(&key_fifo, &key, sizeof(key));
	up(&key_sem);			//�ź�����1
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

	atomic_dec(&open_flag);
	if(atomic_read(&open_flag) == 0)
		disable_irq(IRQ_EINT(19));	
	
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

	/*<������>: �ź�����ʹ��
	  *дfifoǰ�������ź�����1(Ϊ0)
	  *д��fifo��������ź�����1(Ϊ1)
	  */
	down(&key_sem);		//�ź�����1	
	retval = kfifo_in(&key_fifo, &key, sizeof(key));
	up(&key_sem);			//�ź�����1
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

	/*<�����>: ��ʼ���ź�����ֵΪ1*/
	sema_init(&key_sem, 1);

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

	disable_irq(IRQ_EINT(19));	//�жϹر�ʹ��

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


MODULE_LICENSE("Dual BSD/GPL");	//BSD/GPL˫�����֤
MODULE_AUTHOR("sunplusedu");		//ģ������(��ѡ)
MODULE_DESCRIPTION("used for studing linux drivers");	//ģ������(��ѡ)
