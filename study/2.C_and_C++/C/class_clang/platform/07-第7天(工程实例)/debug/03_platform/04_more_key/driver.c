#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <plat/gpio-cfg.h>
#include <mach/gpio.h>
#include <linux/interrupt.h>
#include <linux/irq.h>

static unsigned int major = 0;
static struct class *my_class = NULL;
static struct resource *io_res[10];
static struct resource *irq_res[10];
static struct resource *mem_res0 = NULL;
static struct resource *mem_res1 = NULL;

static int driver_open(struct inode *i_node, struct file *f_ile)
{
	printk(KERN_CRIT "IN %s\n",__FUNCTION__);
	
	return 0;
}

static ssize_t driver_read(struct file *f_ile, char __user *buff, size_t len, loff_t *off_t)
{
	printk(KERN_CRIT "IN %s\n",__FUNCTION__);
	
	return 0;
}

static int driver_release(struct inode *i_node, struct file *f_ile)
{
	printk(KERN_CRIT "IN %s\n",__FUNCTION__);
	
	return 0;
}

static struct file_operations f_ops = {
	.owner = THIS_MODULE,
	.open = driver_open,
	.read = driver_read,
	.release = driver_release,
};

static irqreturn_t my_handle(int irq,void *dev_id)
{
	int i = 0;
	
	do
	{
		if(gpio_get_value(irq_res[i]->start))
		{
			printk(KERN_CRIT "IN %s --%d--\n",__FUNCTION__,i);
			break;
		}
	}while(irq_res[++i]);

	return IRQ_HANDLED;
}

static int driver_probe(struct platform_device *plat_dev)
{
	int ret = 0;
	int i = 0;
	
	printk(KERN_CRIT "IN %s\n",__FUNCTION__);

	major = register_chrdev(0,"zxc",&f_ops);
	my_class = class_create(THIS_MODULE, "my_class");
	device_create(my_class,NULL,MKDEV(major,3),NULL,"my_plat");

	do
	{
		io_res[i] = platform_get_resource(plat_dev,IORESOURCE_IO,i);
	}while(io_res[i++]);
	i = 0;
	do
	{
		irq_res[i] = platform_get_resource(plat_dev,IORESOURCE_IRQ,i);
	}while(irq_res[i++]);
	i = 0;
	do
	{
		s3c_gpio_cfgpin(io_res[i]->start,S3C_GPIO_OUTPUT);
		gpio_set_value(io_res[i]->start,1);
	}while(io_res[++i]);
	i = 0;
	do
	{
		s3c_gpio_setpull(irq_res[i]->start,S3C_GPIO_PULL_DOWN);
	}while(irq_res[++i]);
	i = 0;
	do
	{
		ret = request_irq(gpio_to_irq(irq_res[i]->start),my_handle,IRQF_SHARED|IRQ_TYPE_EDGE_BOTH,"my_irq",(void *)"key 2");
	}while(irq_res[++i]);
	
	return 0;
}

static int driver_remove(struct platform_device *plat_dev)
{
	int i = 0;
	printk(KERN_CRIT "IN %s\n",__FUNCTION__);
	unregister_chrdev(major,"zxc");
	device_destroy(my_class,MKDEV(major,3));
	class_destroy(my_class);
	do
	{
		free_irq(gpio_to_irq(irq_res[i]->start),(void *)"key 2");
	}while(irq_res[++i]);
	
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