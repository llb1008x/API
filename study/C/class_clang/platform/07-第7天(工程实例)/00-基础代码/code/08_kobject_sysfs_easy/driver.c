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

static struct kobject *kparent;
static struct kobject *kchild;

static unsigned char test_flag[30] = {0};

static ssize_t att_show(struct kobject *kobj,struct attribute *attr,char *buf)
{
	return sprintf(buf,"%s\n",test_flag);
}

static ssize_t att_store(struct kobject *kobj,struct attribute *attr,const char *buf,size_t count)
{	
	int len;
	len = sprintf(test_flag,"%s\n",buf);
	printk(KERN_WARNING "len = %d\n",len);
	return 0;
}

static struct attribute child_att = {
	.name = "demo_att",
	.mode = S_IRUGO|S_IWUSR,
};

static const struct sysfs_ops sys_ops = {
	.show = att_show,
	.store = att_store,
};

static struct kobj_type child_ktype = {
	.sysfs_ops = &sys_ops,
};

static int __init demo_kobj_init(void)
{
	int ret,

	/* create parent */
	kparent = kzalloc(sizeof(kparent),GFP_KERNEL);
	if(IS_ERR(kparent)){
		ret = PTR_ERR(kparent);
		goto error;
	}
	kobject_init(kparent, &child_ktype);
	ret = kobject_add(kparent,NULL,"parent_obj");
	if(ret)
		goto p_kobject_add_error;
	
	/* create child */
	kchild = kzalloc(sizeof(kchild),GFP_KERNEL);
	if(IS_ERR(kchild)){
		ret = PTR_ERR(kchild);
		goto kzalloc_error;
	}
	kobject_init(kchild,&child_ktype);
	ret = kobject_add(kchild,kparent,"child_obj");
	if(ret)
		goto c_kobject_add_error;
	
	ret = sysfs_create_file(kchild,&child_att);
	if (ret) 
		goto sysfs_create_error;
		
	return 0;
	
sysfs_create_error:	
	kobject_del(kchild);
c_kobject_add_error:	
	kzfree(kchild);
kzalloc_error:	
	kobject_del(kparent);
p_kobject_add_error:
	kzfree(kparent);
error:	
	return ret;
}

static void __exit demo_kobj_exit(void)
{
	printk(KERN_WARNING "%s\n",__FUNCTION__);
	sysfs_remove_file(kchild,&child_att);
	kobject_del(kchild);
	kzfree(kchild);
	kobject_del(kparent);	
	kzfree(kparent);
}

module_init(demo_kobj_init);
module_exit(demo_kobj_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("sunplusedu");

