#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>

static struct resource my_res[] = {
	[0] = {
		.start = S5PV210_GPH3(0),
		.end = 57,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = 66,
		.end = 99,
		.flags = IORESOURCE_IO,
	},
	[2] = {
		.start = 58,
		.end = 58,
		.flags = IORESOURCE_MEM,
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