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
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <asm/atomic.h>

#include <linux/input.h>

#define	DEMO_DEBUG
#ifdef 	DEMO_DEBUG
#define	dem_dbg(fmt, arg...)		printk(KERN_WARNING fmt, ##arg)
#else
#define	dem_dbg(fmt, arg...)		printk(KERN_DEBUG fmt, ##arg)
#endif

#define KEY_CODE	KEY_LEFT

static struct workqueue_struct *key_workqueue;
static struct work_struct key_work;

atomic_t open_flag = ATOMIC_INIT(0);
//�洢�жϺ�
static unsigned int irq;

static struct input_dev *input_key;

static void key_workhandler(struct work_struct *work)
{
	int gpio_val = 0;
	
	dem_dbg("[kern func]: %s  do work handler\n", __FUNCTION__);

	/*��ʱȥ���ĸĽ������λ�ȡ��ƽ������Ƕ���ֱ�ӷ���*/
	gpio_val = gpio_get_value(S5PV210_GPH2(3));
	msleep(50);
	if(gpio_val != gpio_get_value(S5PV210_GPH2(3)))
		return;

	if(gpio_val == 0){	//�㱨�����¼�
		 input_report_key(input_key, KEY_CODE, 1);
		 
	}else{	//�㱨̧���¼�
		 input_report_key(input_key, KEY_CODE, 0);
	}
	//�㱨ͬ���¼�
	input_sync(input_key);
}

irqreturn_t key_irqhandler(int irq, void *dev_id)
{
	dem_dbg("[kern func]: %s  in irq handler\n", __FUNCTION__);
	
	queue_work(key_workqueue, &key_work);

	return IRQ_HANDLED;
}

static int key_open(struct input_dev *dev)
{
	dem_dbg("[kern func]: %s\n", __FUNCTION__);
	
	if(atomic_read(&open_flag) == 0)
		enable_irq(IRQ_EINT(19));		//���ж�
	atomic_inc(&open_flag);
	return 0;
}
static void key_close(struct input_dev *dev)
{
	dem_dbg("[kern func]: %s\n", __FUNCTION__);
	
	atomic_dec(&open_flag);
	if(atomic_read(&open_flag) == 0)
		disable_irq(IRQ_EINT(19));	
}

static int key_probe(struct platform_device *pdev)
{
	int retval;
	struct resource *res = NULL;
	
	dem_dbg("[msg]:this is a driver demo, in module initial function\n");

	/*��ȡplatform_device ��Դ*/
	res = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	if (res == NULL) {
		dem_dbg("[err]: no memory resource specified\n");
		return PTR_ERR(res);
	}

	irq = res->start;

	key_workqueue = create_workqueue("key_workqueue");
	if(IS_ERR(key_workqueue)){
		dem_dbg("[err]:create workqueue failed!\n");
		retval = PTR_ERR(key_workqueue);
		goto wq_err;
	}
	INIT_WORK(&key_work, key_workhandler);

	retval = request_irq(irq, key_irqhandler, IRQ_TYPE_EDGE_BOTH, 
			"demo_key", NULL);
	if(retval){
		dem_dbg("[err]:request eint20 failed!\n");
		goto irq_err;
	}

	disable_irq(irq);	//�жϹر�

	/*<����һ>: ����input_dev�ṹ������ռ�*/
	input_key = input_allocate_device();
	if (!input_key) {
		dem_dbg("[err]: allocate input device failed!\n");
		retval = -ENOMEM;
		goto input_alloc_err;
	}
	
	/*<�����>: input_dev�ṹ�������ʼ��*/
	input_key->name = "key";			//�鿴cat /proc/bus/input/devices
	input_key->phys = "inputkey";
	input_key->id.bustype = BUS_HOST;
	input_key->id.vendor = 0x0001;
	input_key->id.product = 0x0001;
	input_key->id.version = 0x0100;

	//ָ��input�豸��֧�ֵ��¼�:�����¼���ͬ���¼��������������¼�
	input_key->evbit[0] = BIT(EV_KEY) | BIT(EV_SYN) | BIT(EV_REP);

	//ָ��input�豸֧�ֻ㱨�ļ�ֵ: KEY_CODE��ָ����KEY_LEFT
	set_bit(KEY_CODE, input_key->keybit);

	input_key->open = 	key_open;
	input_key->close = key_close;
	
	/*<������>: input_dev�ṹ���ע��*/
	retval = input_register_device(input_key);
	if (retval) {
		dem_dbg("[err]: Failed to register input device!\n");
		goto input_regs_err;
	}
	return 0;
	

input_regs_err:
	input_free_device(input_key);

input_alloc_err:
	free_irq(irq, NULL);
	
irq_err:
	destroy_workqueue(key_workqueue);
	
wq_err:
	return retval;
}

static int key_remove(struct platform_device *dev)
{
	dem_dbg("[msg]:in module exit function\n");

	flush_workqueue(key_workqueue);
	destroy_workqueue(key_workqueue);
	
	free_irq(irq, NULL);

	/*<������>: input_dev�ṹ���ע�����ͷ�*/
	input_unregister_device(input_key);
	input_free_device(input_key);

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


MODULE_LICENSE("Dual BSD/GPL");	//BSD/GPL˫�����֤
MODULE_AUTHOR("sunplusedu");		//ģ������(��ѡ)
MODULE_DESCRIPTION("used for studing linux drivers");	//ģ������(��ѡ)
