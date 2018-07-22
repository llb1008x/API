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
#include <linux/platform_device.h>

extern struct bus_type demo_bus_type;
static void demo_dev_release(struct device *dev)
{
	printk(KERN_WARNING "%s\n",__FUNCTION__);
}
static ssize_t demo_dev_show(struct device *dev, struct device_attribute *attr,char *buf)
{
	return snprintf(buf,PAGE_SIZE,"%s\n","This is demo device!");
}

static struct device demo_dev = {
.init_name = "demo_bus_dev_drv",
.bus = &demo_bus_type,
.release = demo_dev_release,
};
static DEVICE_ATTR(demo_dev,S_IRUGO,demo_dev_show,NULL);
static int __init demo_bus_dev_init(void)
{
	int ret = 0;
	
	ret = device_register(&demo_dev);
	if(ret)
		goto device_register_fail;
	ret = device_create_file(&demo_dev,&dev_attr_demo_dev);
	if(ret)
		goto device_create_file_fail;
	return 0;
device_create_file_fail:
		device_unregister(&demo_dev);
device_register_fail:
	return ret;
}
static void __exit demo_bus_dev_exit(void)
{
	device_remove_file(&demo_dev,&dev_attr_demo_dev);
	device_unregister(&demo_dev);
	return;
}

module_init(demo_bus_dev_init);
module_exit(demo_bus_dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("xiuhai.deng@sunplusapp.com");


