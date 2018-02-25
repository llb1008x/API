/*
 * Copyright (c) 2012 sunplusapp
 * this program is free software; you can redistribute it and/or modify it
 *	
 * this is a demo about how to create bus  
 *
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
static char *Version = "$Rversion:1.1 by lightsalt $";
static int my_match(struct device *dev,struct device_driver *driver)
{
	printk(KERN_INFO "have a try match or not?\n");
	return  !strncmp(dev_name(dev), driver->name, strlen(driver->name));
}

struct bus_type  my_bus_type= {
	.name  =  "my_bus",
	.match  =  my_match,
};

static ssize_t show_bus_version(struct bus_type *bus,char *buf)
{
	ssize_t ret = snprintf(buf,PAGE_SIZE,"%s\n",Version);
	printk(KERN_INFO "buf=*%s*\n", buf);
	return ret;
}

static BUS_ATTR(version, S_IRUGO, show_bus_version, NULL);

static __init int my_bus_init(void)
{
	int ret;
	ret = bus_register(&my_bus_type);
	if(ret)
		return ret;
	if(bus_create_file(&my_bus_type,&bus_attr_version))
		printk(KERN_NOTICE"Fail to create version attribute!\n");
	return 0;
}

static void my_bus_exit(void)
{
	bus_unregister(&my_bus_type);
}

EXPORT_SYMBOL(my_bus_type);

module_init(my_bus_init);
module_exit(my_bus_exit);
MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("sunplusedu"); 
