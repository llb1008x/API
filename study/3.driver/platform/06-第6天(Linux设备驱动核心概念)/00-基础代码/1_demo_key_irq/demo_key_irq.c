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

/*request_irq��������ͷ�ļ�*/
#include <linux/interrupt.h>
/*set_irq_type�������жϺ�ͷ�ļ�*/
#include <linux/irq.h>

/*gpio�����ӿں���*/
#include <mach/gpio.h>
#include <plat/gpio-cfg.h>

#define	DEMO_DEBUG
#ifdef 	DEMO_DEBUG
#define	dem_dbg(fmt, arg...)		printk(KERN_WARNING fmt, ##arg)
#else
#define	dem_dbg(fmt, arg...)		printk(KERN_DEBUG fmt, ##arg)
#endif

#define	DEVICE_COUNT			1

/*ȫ�ֱ�������¼��ֵ*/
static int key_value = -1;

//open���ü�����û��һ�μ���ֵ��һ���ر����෴
static int open_flag = 0;

static int major = 0;
static int demo_open (struct inode *pnode, struct file *filp)
{
	dem_dbg("[kern func]: %s  major: %d  minor: %d\n",
					__FUNCTION__, imajor(pnode), iminor(pnode));
	if(!open_flag)
		enable_irq(IRQ_EINT(19));	//���ж�
	open_flag++;
	
	return 0;
}

static ssize_t demo_read (struct file *filp, char __user *buf, size_t count, loff_t *offp)
{
	int key;
	unsigned long len = min(count, sizeof(key));
	int retval;
	//��ȡȫ�ֱ�������ֵ
	key = key_value;
	if(copy_to_user(buf, &key, len) != 0){
		retval = -EFAULT;
		goto cp_err;
	}
	//��ֵ���
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
		disable_irq(IRQ_EINT(19));		//�ر��ж�
	
	return 0;
}

/*@����file_operations�ṹ�����*/
static struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = demo_read,
	.open = demo_open,
	.release = demo_release,
};	

/*@�жϷ�����
  *   ����紥������ʱ���ú������ձ����ã�����irq
  *   Ϊ��Ӧ���жϺţ�dev_idΪ�����ж�ʱ���ݵ����һ������
  */
irqreturn_t key_irqhandler(int irq, void *dev_id)
{
	dem_dbg("[kern func]: %s  in irq handler\n", __FUNCTION__);
	//���ü�ֵΪ1
	key_value = 1;

	return IRQ_HANDLED;	//ִ�гɹ����ظ�ö��ֵ
}

/*@����io�ڳ�ʼ��
  *  ����������ֻʵ��һ����������������GPH2���������Ŷ�Ӧ����
  */
static void key_ioport_init(void)
{
	//���õ�0��1��Ϊ�����������͵�ƽ
	s3c_gpio_cfgpin(S5PV210_GPH3(0), S3C_GPIO_OUTPUT);
	s3c_gpio_cfgpin(S5PV210_GPH3(1), S3C_GPIO_OUTPUT);
	gpio_set_value(S5PV210_GPH3(0), 0);
	gpio_set_value(S5PV210_GPH3(1), 0);

	//@���õ�һ��Ϊ����ʹ�ܣ�
	//ע��:���ﲻ��Ҫ���ø�������Ϊ�жϹ��ܣ�set_irq_type�����л��������
	s3c_gpio_setpull(S5PV210_GPH2(3), S3C_GPIO_PULL_UP);
}

static struct class *demo_class;
static int __init drvdemo_init(void)
{
	struct device *demo_device;
	int i;
	int retval;
	
	dem_dbg("[msg]:this is a driver demo, in module initial function\n");

	/*��ʼ������IO��*/
	key_ioport_init();
	
	/*�����ⲿ�ж�19*/
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

	for(i=0; i<DEVICE_COUNT; i++){	//���ɴ���255���豸�ڵ�(register_chrdev����������0-254��Χ�Ĵ��豸��)
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
	/*�ͷ�����ɹ����ж�*/
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
