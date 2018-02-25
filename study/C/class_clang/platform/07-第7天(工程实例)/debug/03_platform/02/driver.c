#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>

static struct resource *io_res = NULL;
static struct resource *mem_res0 = NULL;
static struct resource *mem_res1 = NULL;

static int driver_probe(struct platform_device *plat_dev)
{
	printk(KERN_CRIT "IN %s\n",__FUNCTION__);
	io_res = platform_get_resource(plat_dev,IORESOURCE_IO,0);
	printk(KERN_CRIT "res_io.start =  %d\n",io_res->start);
	printk(KERN_CRIT "res_io.end =  %d\n",io_res->end);
	mem_res0 = platform_get_resource(plat_dev,IORESOURCE_MEM,0);
	printk(KERN_CRIT "res_mem0.start =  %d\n",mem_res0->start);
	mem_res1 = platform_get_resource(plat_dev,IORESOURCE_MEM,1);
	printk(KERN_CRIT "res_mem1.start =  %d\n",mem_res1->start);

	return 0;
}

static int driver_remove(struct platform_device *plat_dev)
{
	printk(KERN_CRIT "IN %s\n",__FUNCTION__);

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