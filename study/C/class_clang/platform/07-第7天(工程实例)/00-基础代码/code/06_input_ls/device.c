/*
* Copyright (c) 2014 sunplusedu
* This is a demo,about hwo to create a platform device
* 
* Date and Dition:		2014-03-19 	v1.0
* Author:				<xiuhai.deng@sunplusapp.com>
*
*/

#include <linux/module.h>
#include <linux/ioport.h>	/* resource */
#include <linux/irq.h>	/* IRQ_EINT(23) */
#include <linux/gpio.h>	/* S5PV210_GPH3(0) */
#include <linux/platform_device.h> /* platform_device_register() */

static struct resource demo_key_resource[] = {
	[0]={
		.start = IRQ_EINT(23),
		.end = IRQ_EINT(23),
		.name = "IRQ_EINT_23",
		.flags = IORESOURCE_IRQ,
	},
	[1]={
		.start = S5PV210_GPH2(7),
		.end = S5PV210_GPH2(7),
		.name = "GPH2_7",
		.flags = IORESOURCE_IO,
	},
	[2]={
		.start = S5PV210_GPH3(0),
		.end = S5PV210_GPH3(0),
		.name = "GPH3_0",
		.flags = IORESOURCE_IO,
	},
	[3]={
		.start = S5PV210_GPH0(3),
		.end = S5PV210_GPH0(3),
		.name = "GPH0_3",
		.flags = IORESOURCE_IO,
	},
	
};

void platform_demo_key_release(struct device *dev)
{
	printk(KERN_WARNING "%s\n",__FUNCTION__);
	return;
}

static struct platform_device device_demo_key = {
.name = "s5pv210-key",
.id = -1,
.num_resources = ARRAY_SIZE(demo_key_resource),
.resource = demo_key_resource,
.dev = {
	.release = platform_demo_key_release,
}
};

static int __init platform_dev_init(void)
{
	return platform_device_register(&device_demo_key);
}
static void __exit platform_dev_exit(void)
{
	platform_device_unregister(&device_demo_key);
	return;
}

module_init(platform_dev_init);
module_exit(platform_dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("xiuhai.deng@sunplusapp.com");


