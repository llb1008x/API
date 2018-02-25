/*
* Copyright (c) 2014 sunplusedu
* This is a demo, about hwo to create a module
*
* Date and Dition:	2014-03-12	v1.0
* Author:				<xiuhai.deng@sunplusapp.com>
* 
*/

/*
* debug:
* 		cat xxx > /dev/demo_dev
* 		cat /dev/demo_dev
*/

#include <linux/module.h>	/* module_init */
#include <linux/fs.h>	/* file_operations */
#include <linux/device.h>	/* class device */
#include <linux/sched.h>		/* current */
#include <linux/mount.h>		/* struct vfsmount */
#include <asm/io.h>	/* writel() */
#include <plat/gpio-cfg.h>	/* s3c_gpio_cfgpin */
#include <mach/gpio.h>	/* gpio_set_value */
#include <linux/uaccess.h> /* copy_to_user() */
#include <linux/slab.h> /* kzalloc() */
#include <linux/err.h>


extern struct bus_type demo_bus_type;

static int demo_drv_probe(struct device *dev)
{
	printk(KERN_WARNING "%s\n",__FUNCTION__);
	return 0;
}
static int demo_drv_remove(struct device *dev)
{	
	printk(KERN_WARNING "%s\n",__FUNCTION__);
	return 0;
}

static ssize_t demo_drv_show(struct device_driver *driver,char *buf)
{
	return snprintf(buf,PAGE_SIZE,"%s\n","This is demo driver!");
}
static struct device_driver demo_drv = {
.name = "demo_bus_dev_drv",
.bus = &demo_bus_type,
.probe = demo_drv_probe,
.remove = demo_drv_remove,
};

static DRIVER_ATTR(demo_drv,S_IRUGO,demo_drv_show,NULL);
static int __init demo_bus_drv_init(void)
{
	int ret = 0;
	
	ret = driver_register(&demo_drv);
	if(ret)
		goto driver_register_fail;
	ret = driver_create_file(&demo_drv,&driver_attr_demo_drv);
	if(ret)
		goto driver_create_file_fail;	

return 0;
driver_create_file_fail:
	driver_unregister(&demo_drv);
driver_register_fail:
	return ret;
}

static void __exit demo_bus_drv_exit(void)
{
	printk(KERN_WARNING "%s\n",__FUNCTION__);
	driver_remove_file(&demo_drv,&driver_attr_demo_drv);
	driver_unregister(&demo_drv);
}

module_init(demo_bus_drv_init);
module_exit(demo_bus_drv_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("sunplusedu");

