#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <plat/gpio-cfg.h>
#include <mach/gpio.h>

static struct resource my_res[] = {
	[0] = {
		.start = S5PV210_GPH3(1),
		.end = S5PV210_GPH3(1),
		.flags = IORESOURCE_IO,
	},
	[1] = {
		.start = S5PV210_GPH2(6),
		.end = S5PV210_GPH2(6),
		.flags = IORESOURCE_IRQ,
	},
	[2] = {
		.start = S5PV210_GPH3(0),
		.end = S5PV210_GPH3(0),
		.flags = IORESOURCE_IO,
	},
	[3] = {
		.start = S5PV210_GPH2(3),
		.end = S5PV210_GPH2(3),
		.flags = IORESOURCE_IRQ,
	},
	[4] = {
		.start = S5PV210_GPH3(0),
		.end = S5PV210_GPH3(0),
		.flags = IORESOURCE_IO,
	},
	[5] = {
		.start = S5PV210_GPH2(5),
		.end = S5PV210_GPH2(5),
		.flags = IORESOURCE_IRQ,
	},
};

static void device_release(struct device *dev)
{
	printk(KERN_CRIT "IN %s\n",__FUNCTION__);
}

static struct platform_device plat_dev = {
	.name = "my_platform_driver",
	.id = 0,
	.resource = my_res,
	.num_resources = ARRAY_SIZE(my_res),
	.dev = {
		.release = device_release,
	},
};

static int __init device_init(void)
{
	platform_device_register(&plat_dev);

	return 0;
}

static void __exit device_exit(void)
{
	platform_device_unregister(&plat_dev);
}

module_init(device_init);
module_exit(device_exit);

MODULE_AUTHOR("SUNPLUS");
MODULE_LICENSE("GPL");