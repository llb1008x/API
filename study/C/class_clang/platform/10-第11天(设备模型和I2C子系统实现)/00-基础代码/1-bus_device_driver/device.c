/*
 * Copyright (c) 2012 sunplusapp
 * this program is free software; you can redistribute it and/or modify it
 *	
 * this is a demo about how to create a device
 * 
 *
 * Date and Edition:		2012-11-18  v1.0
 * Date and Edition:		2013-01-07  v1.1
 * Author:				<feng.lv@sunplusapp.com>
 */


#include <linux/device.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/stat.h>

extern struct bus_type my_bus_type;

static void my_dev_release(struct device *dev)
{
	printk("my_dev_release!\n");
}
struct device my_dev = {
	.init_name  =  "my_dev_drv", 
	.bus   =  &my_bus_type,
	.release = my_dev_release,
};

static ssize_t mydev_show(struct device *dev,char *buf)
{
	return sprintf(buf,"%s\n","This is my device!");
}

static DEVICE_ATTR(dev, S_IRUGO, mydev_show, NULL);

static int __init my_bus_dev_init(void)
{
	int ret = 0;

	if ((ret = device_register(&my_dev)) < 0) {
		my_dev.bus = NULL;
		goto error;
	}
		
	if ((ret = device_create_file(&my_dev, &dev_attr_dev)) < 0) {
		device_remove_file(&my_dev, &dev_attr_dev);
		goto error;
	}
	
	return 0;
error:
	return ret;
}

static void my_bus_dev_exit(void)
{
	device_unregister(&my_dev);
}

module_init(my_bus_dev_init);
module_exit(my_bus_dev_exit);
MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("sunplusedu"); 
