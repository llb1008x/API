/*
 * Copyright (c) 2012 sunplusapp
 * this program is free software; you can redistribute it and/or modify it
 *	
 * this is a demo about how to create a driver
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
static int my_probe(struct device *dev )
{
	printk("congratulate to you !!! \t Driver found device which my driver can handle!\n");
	/* you can create your interface with ap 
	* such as file_operations or input_dev
	*/
	return 0;
}
static int my_remove(struct device *dev)
{
	printk("Driver found device unpluged!\n");
	return 0;
}
struct device_driver my_driver = {
	.name = "my_dev_drv",
	.bus  = &my_bus_type,
	.probe = my_probe,
	.remove = my_remove,
};

static ssize_t mydriver_show(struct device_driver *driver,char *buf)
{
	return sprintf(buf,"%s\n","This is my driver!");
}

static DRIVER_ATTR(drv, S_IRUGO, mydriver_show, NULL);
static int __init my_driver_init(void)
{
	int ret = 0;
	
	if ((ret = driver_register(&my_driver)) < 0) {
		my_driver.bus = NULL;
		goto error;
	}
	
	if ((ret = driver_create_file(&my_driver, &driver_attr_drv)) < 0) {
		driver_remove_file(&my_driver, &driver_attr_drv);
		goto error;
	}

	return 0;
error:
	return ret;
}

static void my_driver_exit(void)
{
	driver_unregister(&my_driver);
}

module_init(my_driver_init);
module_exit(my_driver_exit);
MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("sunplusedu"); 
