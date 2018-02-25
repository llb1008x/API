#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>

static int driver_probe(struct platform_device *plat_dev)
{
	printk(KERN_CRIT "IN %s",__FUNCTION__);

	return 0;
}

static int driver_remove(struct platform_device *plat_dev)
{
	printk(KERN_CRIT "IN %s",__FUNCTION__);

	return 0;
}

static struct platform_driver plat_drv = {
	.probe = driver_probe,
	.remove = driver_remove,
	.driver = {
		.name = "my_platform_driver",
		.owner = THIS_MODULE,
	},
};

static int __init my_driver_init(void)
{
	platform_driver_register(&plat_drv);

	return 0;
}

static void __exit my_driver_exit(void)
{
	platform_driver_unregister(&plat_drv);
}

module_init(my_driver_init);
module_exit(my_driver_exit);

MODULE_AUTHOR("SUNPLUS");
MODULE_LICENSE("GPL");