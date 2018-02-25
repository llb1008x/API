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

/*request_irq函数声明头文件*/
#include <linux/interrupt.h>
/*set_irq_type函数及中断号头文件*/
#include <linux/irq.h>

/*gpio操作接口函数*/
#include <mach/gpio.h>
#include <plat/gpio-cfg.h>

#define	DEMO_DEBUG
#ifdef 	DEMO_DEBUG
#define	dem_dbg(fmt, arg...)		printk(KERN_WARNING fmt, ##arg)
#else
#define	dem_dbg(fmt, arg...)		printk(KERN_DEBUG fmt, ##arg)
#endif

#define	DEVICE_COUNT			1

/*全局变量，记录键值*/
static int key_value = -1;

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
	//读取全局变量，键值
	key = key_value;
	if(copy_to_user(buf, &key, len) != 0){
		retval = -EFAULT;
		goto cp_err;
	}
	//键值清掉
	key_value = -1;
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

/*@定义file_operations结构体变量*/
static struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = demo_read,
	.open = demo_open,
	.release = demo_release,
};	

/*@中断服务函数
  *   当外界触发按键时，该函数最终被调用，其中irq
  *   为对应的中断号，dev_id为申请中断时传递的最后一个参数
  */
irqreturn_t key_irqhandler(int irq, void *dev_id)
{
	dem_dbg("[kern func]: %s  in irq handler\n", __FUNCTION__);
	//设置键值为1
	key_value = 1;

	return IRQ_HANDLED;	//执行成功返回该枚举值
}

/*@按键io口初始化
  *  该例子我们只实现一个按键的驱动，即GPH2第三个引脚对应按键
  */
static void key_ioport_init(void)
{
	//配置第0、1行为输出，且输出低电平
	s3c_gpio_cfgpin(S5PV210_GPH3(0), S3C_GPIO_OUTPUT);
	s3c_gpio_cfgpin(S5PV210_GPH3(1), S3C_GPIO_OUTPUT);
	gpio_set_value(S5PV210_GPH3(0), 0);
	gpio_set_value(S5PV210_GPH3(1), 0);

	//@配置第一列为上拉使能，
	//注意:这里不需要配置该列引脚为中断功能，set_irq_type函数中会进行设置
	s3c_gpio_setpull(S5PV210_GPH2(3), S3C_GPIO_PULL_UP);
}

static struct class *demo_class;
static int __init drvdemo_init(void)
{
	struct device *demo_device;
	int i;
	int retval;
	
	dem_dbg("[msg]:this is a driver demo, in module initial function\n");

	/*初始化按键IO口*/
	key_ioport_init();
	
	/*申请外部中断19*/
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

	for(i=0; i<DEVICE_COUNT; i++){	//最多可创建255个设备节点(register_chrdev函数会申请0-254范围的从设备号)
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
	return retval;
}

static void __exit drvdemo_exit(void)
{
	int i;
	
	dem_dbg("[msg]:in module exit function\n");
	/*释放申请成功的中断*/
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
