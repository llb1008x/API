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

#include <linux/module.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/kernel.h>
#include <linux/irq.h>
#include <asm/irq.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>


struct resource demokey_resource[] = {
		[0] = {
			.start = IRQ_EINT(19),
			.end = IRQ_EINT(19),
			.flags = IORESOURCE_IRQ,
		},
		[1] = {
			.start = S5PV210_GPH3(0),
			.end = S5PV210_GPH3(0),
			.flags = IORESOURCE_IO
		}
};

static void platform_demokey_release(struct device *dev) 
{
	printk("%s\n", __func__);
	return ;
}

struct platform_device device_demokey = {
	.name = "s5pv210-key",
	.id = -1,
	.num_resources = ARRAY_SIZE(demokey_resource),
	.resource = demokey_resource,
	.dev = {
		.release = platform_demokey_release,
	}
};


static int  __init platform_dev_init(void)
{
	return platform_device_register(&device_demokey);

}

static void  __exit platform_dev_exit(void)
{
	platform_device_unregister(&device_demokey);
	return ;
}

module_init(platform_dev_init);
module_exit(platform_dev_exit);

MODULE_LICENSE("Dual BSD/GPL");	//BSD/GPL双重许可证
MODULE_AUTHOR("sunplusedu");		//模块作者(可选)
MODULE_DESCRIPTION("used for studing linux drivers");	//模块儿简介(可选)

