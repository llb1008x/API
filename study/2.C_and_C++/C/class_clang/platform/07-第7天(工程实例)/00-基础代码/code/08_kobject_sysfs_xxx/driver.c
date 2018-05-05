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

/* 
在/sys下建一个pa_obj目录，再在目录下建两个子目录
cld_obj和c_kset，然后给cld_obj一个属性文件demo_att
*/
static struct kobject *parent;
static struct kobject *child;
static struct kset *c_kset;

static unsigned char flag[30];

static ssize_t att_show(struct kobject *kobj,struct attribute *attr,char *buf)
{
	return sprintf(buf,"%s\n",flag);
}

static ssize_t att_store(struct kobject *kobj,struct attribute *attr,const char *buf,size_t count)
{	
	return sprintf(flag,"%s\n",buf);
}

static struct attribute cld_att = {
	.name = "demo_att",
	.mode = S_IRUGO|S_IWUSR,
};

static const struct sysfs_ops att_ops = {
	.show = att_show,
	.store = att_store,
};

static struct kobj_type cld_ktype = {
	.sysfs_ops = &att_ops,
};

static int __init demo_kobj_init(void)
{
	int ret,
		
	/*create a parent kobject*/
	parent = kobject_create_and_add("pa_obj",NULL);
	if(IS_ERR(parent)){
		ret = PTR_ERR(parent);
		goto error;
	}
	child = kzalloc(sizeof(*child),GFP_KERNEL);
	if(IS_ERR(child)){
		ret = PTR_ERR(child);
		goto kzalloc_error;
	}
	/*create a client_kset attach pa_obj*/
	c_kset = kset_create_and_add("c_kset", NULL, parent);
	if(IS_ERR(c_kset)){
		ret = PTR_ERR(c_kset);
		goto kset_error;
	}
	child->kset = c_kset; /*let clild->kset attach c_kset*/
	
	/*init cld_kobject with kobj_type, whose parent is pa_obj*/
	ret = kobject_init_and_add(child, &cld_ktype, parent, "cld_obj");
	if(ret)
		goto kobject_init_error;
		
	/*create a sysfile*/
	ret = sysfs_create_file(child,&cld_att);
	if (ret) 
		goto kobject_init_error;
		
	return 0;
	
kobject_init_error:	
	kset_unregister(c_kset);
	kobject_del(child);
kset_error:	
	kzfree(child);
kzalloc_error:	
	kobject_del(parent);
error:	
	return ret;
}

static void __exit demo_kobj_exit(void)
{
	sysfs_remove_file(child,&cld_att);
	kset_unregister(c_kset);
	kobject_del(child);
	kzfree(child);
	kobject_del(parent);
}

module_init(demo_kobj_init);
module_exit(demo_kobj_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("sunplusedu");

