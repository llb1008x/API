
/*
 * Copyright  2011-2012  sunplusapp
 *
 * key15 device driver - using kthread
 */

/*
 * this program is free software; you can redistribute it and/or modify
 * it
 *
 * the key1*5 use GPH2 and GPH3 io ports; there are two raws and five
 * lines, the raws connect to GPH3, the lines connect to GPH2; we can
 * realize the key1*5 by setting the GPH3 pin low level
 *
 *
 * in the example we use kthread to check the key value, and realize the
 * block-read
 * 
 * Date and Edition:		2012-12-27  v1.3
 * Author:				zsd/Valor-Lion 
 */

 
#include <linux/module.h>    	/*module_init()*/
#include <linux/kernel.h>		/* printk() */
#include <linux/init.h>			/* __init __exit */
#include <linux/fs.h>			/* file_operation */
#include <asm/uaccess.h>		/* copy_to_user, copy_from_user */
#include <linux/device.h> 	 	/*class ,class_create ,device_create 等*/
#include <linux/errno.h>		/* Error number */
#include <linux/delay.h>		/* udelay */
#include <mach/regs-gpio.h>  	
#include <linux/types.h>

#include <asm/io.h>
#include <mach/gpio.h>
#include <linux/kthread.h>


#define DRIVER_NAME				"s5pv210-key15"
#define err(msg) 				printk(KERN_ERR "%s: " msg "\n", DRIVER_NAME)
#define __debug(fmt, arg...)	printk(KERN_DEBUG fmt, ##arg)

#define GPH2CON					(unsigned long)(S5PV210_GPH2_BASE+ 0x00)
#define GPH2DAT					(unsigned long)(S5PV210_GPH2_BASE + 0x04)
#define GPH2UP					(unsigned long)(S5PV210_GPH2_BASE + 0x08)
#define GPH2DRV					(unsigned long)(S5PV210_GPH2_BASE+ 0x0C)

#define GPH3CON					(unsigned long)(S5PV210_GPH3_BASE+ 0x00)
#define GPH3DAT					(unsigned long)(S5PV210_GPH3_BASE + 0x04)
#define GPH3UP					(unsigned long)(S5PV210_GPH3_BASE + 0x08)
#define GPH3DRV					(unsigned long)(S5PV210_GPH3_BASE+ 0x0C)



static int major = 0;		/* Driver Major Number */
static int minor = 0;		/* Driver Minor Number */
struct class *key15_class;
static struct device *key15_device;

static wait_queue_head_t wait_queue;
static struct task_struct *key_thread;		//线程指针
static unsigned char key;
/*思考:可否将上面全局变量封装为结构体，让代码
 * 看起来更规范，请自行改进......
 */



static int key_conversion(int key_vlaue)
{
	int key;
	switch(key_vlaue){
		case 0xf0:	//1111 0000
			key=2;
			break;
		case 0xe8:	//1110 1000
			key=3;
			break;
		case 0xd8:	//1101 1000
			key=9;
			break;
		case 0xb8:
			key=10;
			break;
		case 0x78:
			key=11;
			break;
		default:
			break;
	}
	return key;
}

int keynum_scan(void)
{
	int dat_val;
	int key = -1;
	
	dat_val = readl(GPH2DAT);
	//if there has a key pressed
	if((dat_val & 0xf8) != 0xf8){
		msleep(100);
		if((readl(GPH2DAT) & 0xf8)!=0xf8){
			key = key_conversion(dat_val);
		}
	}
	return key;
}

static int key_thread_func(void *data)
{
	unsigned char *pkey = (unsigned char *)data;
	int key;
	
	for(;;){					//while循环实现线程任务
		if(kthread_should_stop())
			break;
		key = keynum_scan();
		if(key >= 0){
			*pkey = key;
			wake_up_interruptible(&wait_queue);			//唤醒等待队列
		}
		schedule_timeout(1);
	}
	return 0;
}

static ssize_t key15_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	int keynum;
	int cpy_len;
	int retval;

	interruptible_sleep_on(&wait_queue);	

	keynum = key;
	cpy_len = min(sizeof(keynum), count);
	retval = copy_to_user(buf, &keynum, cpy_len);
	
	return (cpy_len - retval);
}


/* Driver Operation structure */
static struct file_operations key15_fops = {
	owner:		THIS_MODULE,
	read:		key15_read,
};


static int io_port_init(void)
{
	unsigned long reg_val;
	
	//GPH3  0  1为输出功能，数据初始化为0
	reg_val = readl(GPH3CON);
	reg_val &= ~((0x0f<<0) | (0x0f<<4));
	reg_val |= ((0x01<<0) | (0x01<<4));
	writel(reg_val, GPH3CON);


	reg_val = readl(GPH3DAT);
	reg_val &= ~((0x01<<0) | (0x01<<1));
	writel(reg_val, GPH3DAT);
	
	//GPH2  3 4 5 6 7  为输入功能，上拉使能，驱动能力为2级
	reg_val = readl(GPH2CON);
	reg_val &= ~((0x0f<<12) | (0x0f<<16) | (0x0f<<20) | (0x0f<<24) | (0x0f<<28));
	writel(reg_val, GPH2CON);

	reg_val = readl(GPH2UP);
	reg_val &= ~((0x03<<6) | (0x03<<8) | (0x03<<10) | (0x03<<12) | (0x03<<14));
	reg_val |= ((0x02<<6) | (0x02<<8) | (0x02<<10) | (0x02<<12) | (0x02<<14));
	writel(reg_val, GPH2UP);

	reg_val = readl(GPH2DRV);
	reg_val &= ~((0x03<<6) | (0x03<<8) | (0x03<<10) | (0x03<<12) | (0x03<<14));
	reg_val |= ((0x02<<6) | (0x02<<8) | (0x02<<10) | (0x02<<12) | (0x02<<14));
	writel(reg_val, GPH2DRV);
	
	__debug("key15 open called!\n");
	return 0;
}

/* Module Init & Exit function */
static int __init key15_module_init(void)
{
	int retval = 0;
	
	/* Module init code */
	__debug("key15_init\n");

	io_port_init();
	
	init_waitqueue_head(&wait_queue);			//初始化等待队列头


	/*创建内核线程*/
	key_thread = kthread_create(key_thread_func, &key, "key15_task");
	if(IS_ERR(key_thread)){
		err("create kernel thread failed\n");
		retval = PTR_ERR(key_thread);
		goto error;
	}
	/*运行内核线程*/
	wake_up_process(key_thread);

	
	/* Driver register */
	major = register_chrdev(major, DRIVER_NAME, &key15_fops);
	if(major < 0){
		err("register char device fail!\n");
		retval = major;
		goto error;
	}
	key15_class=class_create(THIS_MODULE,DRIVER_NAME);
	if(IS_ERR(key15_class)){
		err("class create failed!");
		retval =  PTR_ERR(key15_class);
		goto error_class;
	}
	key15_device=device_create(key15_class,NULL, MKDEV(major, minor), NULL,DRIVER_NAME);
	if(IS_ERR(key15_device)){
		err("device create failed!");
		retval = PTR_ERR(key15_device);
		goto error_device;
	}
	__debug("register myDriver OK! Major = %d\n", major);
	return 0;

error_device:
	class_destroy(key15_class);
error_class:
	unregister_chrdev(major, DRIVER_NAME);
error:
	return retval;
}

static void __exit key15_module_exit(void)
{
	/* Module exit code */
	__debug("key15_exit\n");

	kthread_stop(key_thread);				//线程停止函数
	/* Driver unregister */
	unregister_chrdev(major, DRIVER_NAME);
	device_destroy(key15_class,MKDEV(major, minor));
	class_destroy(key15_class);

	return;
}

module_init(key15_module_init);
module_exit(key15_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("sunplusedu");
