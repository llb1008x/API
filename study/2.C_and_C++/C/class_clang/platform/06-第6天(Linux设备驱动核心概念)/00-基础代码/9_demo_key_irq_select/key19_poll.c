
/*
 * Copyright  2011-2012  sunplusapp
 *
 * key using external interrupt driver - using GPHx I/O port
 */

/*
 * this program is free software; you can redistribute it and/or modify
 * it
 *
 * the program realize one button device using external interrupt; the
 * demo can help us study and use the interrupt system; we can use
 * this when building the other driver codes
 * 
 * this code support block-read; if there is no active value, the system-call
 * will not return; we realize this through poll
 * 
 *
 * Date and Edition:		2012-12-25  v2.0
 * Author:				dxh/Valor-Lion/zsd 
 */
 
#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/irq.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <mach/gpio.h>
#include <mach/regs-gpio.h>  

#include <linux/poll.h>  //poll_wait()

#define EINT_DEVICE_ID			1

#define DRIVER_NAME				"key19_eint"
#define err(msg) 				printk(KERN_ERR "%s: " msg "\n", DRIVER_NAME)
#define __debug(fmt, arg...)	printk(KERN_DEBUG fmt, ##arg)

#define GPH3CON					(unsigned long)(S5PV210_GPH3_BASE+ 0x00)
#define GPH3DAT					(unsigned long)(S5PV210_GPH3_BASE + 0x04)

#define GPH2UP					(unsigned long)(S5PV210_GPH2_BASE + 0x08)

static int major = 0;		
static int minor = 0;		
struct class *key_class;
static struct device *key_device;

/*定义等待队列头，该等待队列头属于该驱动程序*/
static wait_queue_head_t wait_queue;
static unsigned char key;

irqreturn_t buttons_interrupt(int irq, void *dev_id)
{	
	key = (unsigned int)dev_id;

	wake_up_interruptible(&wait_queue);			//唤醒等待队列
	return IRQ_HANDLED;
}

static void key_io_port_init(void)
{
	unsigned long reg_val;
	
	reg_val = readl(GPH3CON);
	reg_val &= ~((0x0f<<0) | (0x0f<<4));
	reg_val |= ((0x01<<0) | (0x01<<4));
	writel(reg_val, GPH3CON);

	reg_val = readl(GPH3DAT);
	reg_val &= ~((0x01<<0) | (0x01<<1));
	writel(reg_val, GPH3DAT);

	reg_val = readl(GPH2UP);
	reg_val &= ~(0x03<<8);
	reg_val |= 0x02<<8;
	writel(reg_val, GPH2UP);
}

static ssize_t key_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	int key_num;
	int cpy_len;
	int retval;

	key_num = key;		//读取键值
	key = 0;
	cpy_len = min(sizeof(key_num), count);
	retval = copy_to_user(buf, &key_num, cpy_len);
	
	return (cpy_len - retval);
}

static unsigned int key_poll(struct file *file, struct poll_table_struct *wait)
{
	/*将当前进程添加到队列列表中，但不挂起该进程*/
	poll_wait(file, &wait_queue, wait);
	/*根据当前的键值返回适当的状态掩码*/
	
	if(key == 0)
	return 0;  	//表示没有数据可读/可写/错误
	else
	return POLLIN | POLLRDNORM; //表示有数据可读
}

/* Driver Operation structure */
static struct file_operations key_fops = {
	.owner = THIS_MODULE,
	.read = key_read,
	.poll = key_poll,
};


static int __init key_eint_init(void)
{
	int retval;
	
	key_io_port_init();

	init_waitqueue_head(&wait_queue);			//初始化等待队列头
	
	retval = set_irq_type(IRQ_EINT(20),IRQ_TYPE_EDGE_FALLING);
	if(retval){
		err("IRQ_EINT20 set irq type failed");
		goto error;
	}
	
	retval = request_irq(IRQ_EINT(20), buttons_interrupt, IRQF_DISABLED, 
			"KEY1", (void *)EINT_DEVICE_ID);
	if(retval){
		err("request eint20 failed");
		goto error;
	}
		
	major = register_chrdev(major, DRIVER_NAME, &key_fops);
	if(major < 0){
		err("register char device fail");
		retval = major;
		goto error_register;
	}
	
	key_class=class_create(THIS_MODULE,DRIVER_NAME);
	if(IS_ERR(key_class)){
		err("class create failed!");
		retval =  PTR_ERR(key_class);
		goto error_class;
	}
	
	key_device=device_create(key_class,NULL, MKDEV(major, minor), NULL,DRIVER_NAME);
	if(IS_ERR(key_device)){
		err("device create failed!");
		retval = PTR_ERR(key_device);
		goto error_device;
	}
	__debug("register myDriver OK! Major = %d\n", major);
	return 0;

error_device:
	class_destroy(key_class);
error_class:
	unregister_chrdev(major, DRIVER_NAME);
error_register:
	free_irq(IRQ_EINT(20), (void *)EINT_DEVICE_ID);
error:
	return retval;
}

static void __exit key_eint_exit(void)
{
	//__debug("in key_eint_exit\n");
	
	free_irq(IRQ_EINT(20), (void *)EINT_DEVICE_ID);

	unregister_chrdev(major, DRIVER_NAME);
	device_destroy(key_class,MKDEV(major, minor));
	class_destroy(key_class);

	return;
}

module_init(key_eint_init);
module_exit(key_eint_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("sunplsuedu");

