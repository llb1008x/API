/*
* Copyright (c) 2014 sunplusedu
* This is a demo, about hwo to create a bus
*
* Date and Dition:	2014-03-21	v1.0
* Author:				<xiuhai.deng@sunplusapp.com>
* 
*/


#include <linux/device.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/stat.h>


static char *Version = "$Rversion:1.1 by sunplusedu$";
static int demo_match(struct device *dev,struct device_driver *driver)
{
	printk(KERN_WARNING "%s\n",__FUNCTION__);
	return  !strncmp(dev_name(dev), driver->name, strlen(driver->name));
}

static struct bus_type  demo_bus_type= {
	.name  =  "demo_bus",
	.match  =  demo_match,
};

static ssize_t show_bus_version(struct bus_type *bus,char *buf)
{
	ssize_t ret = snprintf(buf,PAGE_SIZE,"%s\n",Version);
	printk(KERN_WARNING "buf=*%s*\n", buf);
	return ret;
}

static BUS_ATTR(version, S_IRUGO, show_bus_version, NULL);

static int __init demo_bus_init(void)
{
	int ret;
	ret = bus_register(&demo_bus_type);
	if(ret)
		return ret;
	if(bus_create_file(&demo_bus_type,&bus_attr_version))
		printk(KERN_WARNING "Fail to create version attribute!\n");
	return 0;
}

static void __exit demo_bus_exit(void)
{
	bus_remove_file(&demo_bus_type,&bus_attr_version);
	bus_unregister(&demo_bus_type);
}

EXPORT_SYMBOL(demo_bus_type);

module_init(demo_bus_init);
module_exit(demo_bus_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("sunplusedu");

