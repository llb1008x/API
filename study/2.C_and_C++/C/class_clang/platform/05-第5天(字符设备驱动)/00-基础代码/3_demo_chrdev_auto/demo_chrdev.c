/*
 * Copyright  2011-2012  sunplusapp
 * driver demo for sunplusedu
 */

/*
 * this program is free software; you can redistribute it and/or modify
 * it
 * 
 * Date and Edition:		2012-12-27  v1.3
 * Date and Edition:		2013-12-02  v2.0
 * Author:				"driving team" of sunplusedu
 */

#include <linux/init.h>	
#include <linux/module.h>
#include <linux/kernel.h>
/*结构体file_operations定义的头文件*/
#include <linux/fs.h>
/*声明copy_to/from_user函数的头文件*/
#include <linux/uaccess.h>
/*声明class_create 和device_create相关信息*/
#include <linux/device.h>

#define	DEMO_DEBUG
#ifdef 	DEMO_DEBUG
#define	dem_dbg(fmt, arg...)		printk(KERN_WARNING fmt, ##arg)
#else
#define	dem_dbg(fmt, arg...)		printk(KERN_DEBUG fmt, ##arg)
#endif

#define	DEVICE_COUNT			2


/*记录当前驱动所占用的主设备号*/
static int major = 0;

static int demo_open (struct inode *pnode, struct file *filp)
{
	dem_dbg("[kern func]: %s  major: %d  minor: %d\n",
					__FUNCTION__, imajor(pnode), iminor(pnode));
	return 0;
}

static ssize_t demo_read (struct file *filp, char __user *buf, size_t count, loff_t *offp)
{
	unsigned char ary[100] = "you are reading successfully!";
	unsigned long len = min(count, sizeof(ary));	//min是个宏，用来获取两个数中较小的值
	int retval;

	dem_dbg("[kern func]: %s  major: %d  minor: %d\n",
					__FUNCTION__, imajor(filp->f_dentry->d_inode), 
					iminor(filp->f_dentry->d_inode));
	
	//file结构体的f_flags成员可用来判断是否阻塞读取，然后进行相应处理

	if(copy_to_user(buf, ary, len) != 0){
		retval = -EFAULT;
		goto cp_err;
	}

	return len;	//成功返回实际传输的字节数
cp_err:
	return retval;	
}

static ssize_t demo_write(struct file *filp, const char __user *buf, size_t count, loff_t *offp)
{
	unsigned char ary[100] = "";
	unsigned long len = min(count, sizeof(ary));	//min是个宏，用来获取两个数中较小的值
	int retval;

	dem_dbg("[kern func]: %s  major: %d  minor: %d\n",
					__FUNCTION__, imajor(filp->f_dentry->d_inode), 
					iminor(filp->f_dentry->d_inode));
	
	if(copy_from_user(ary, buf, len) != 0){
		retval = -EFAULT;
		goto cp_err;
	}

	printk("[msg]: writing context: %s\n",ary);

	return len;	//成功返回实际传输的字节数
cp_err:
	return retval;	
}

static int demo_release (struct inode *pnode, struct file *filp)
{
	dem_dbg("[kern func]: %s  major: %d  minor: %d\n",
					__FUNCTION__, imajor(pnode), iminor(pnode));
	return 0;
}


/*@定义file_operations结构体变量*/
static struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = demo_read,
	.write = demo_write,
	.open = demo_open,
	.release = demo_release,
};	

static struct class *demo_class;
static int __init drvdemo_init(void)
{
	struct device *demo_device;
	int i;
	int retval;
	
	dem_dbg("[msg]:this is a driver demo, in module initial function\n");

	/*注册字符驱动函数，成功返回动态分配好的主设备号，失败
	 *返回错误码(负值)*/
	major = register_chrdev(0, "demo_chrdev", &fops);
	if(major < 0){
		retval = major;
		goto chrdev_err;
	}

	/*创建设备类（设备模型相关知识，后期课程有介绍）*/
	demo_class = class_create(THIS_MODULE,"demo_class");
	if(IS_ERR(demo_class)){
		retval =  PTR_ERR(demo_class);
		goto class_err;
	}

	/*创建设备文件，通知用户在“/dev/”目录下创件名字为demoX的设备文件*/
	for(i=0; i<DEVICE_COUNT; i++){	//最多可创建255个设备节点(register_chrdev函数会申请0-254范围的从设备号)
		demo_device = device_create(demo_class,NULL, MKDEV(major, i), NULL,"demo%d",i);
		if(IS_ERR(demo_device)){
			retval = PTR_ERR(demo_device);
			goto device_err;
		}
	}
	return 0;
	
device_err:
	while(i--)	//设备节点创建的回滚操作
		device_destroy(demo_class,MKDEV(major, i));
	class_destroy(demo_class);	//删除设备类
class_err:
	unregister_chrdev(major, "demo_chrdev");
chrdev_err:
	return retval;
}

static void __exit drvdemo_exit(void)
{
	int i;
	
	dem_dbg("[msg]:in module exit function\n");
	/*注销字符驱动函数，无返回值，major为已分配的主设备号*/
	unregister_chrdev(major, "demo_chrdev");
	/*删除设备节点和设备类*/
	for(i=0; i<DEVICE_COUNT; i++)
		device_destroy(demo_class,MKDEV(major, i));
	class_destroy(demo_class);
}

module_init(drvdemo_init);
module_exit(drvdemo_exit);


MODULE_LICENSE("Dual BSD/GPL");	//BSD/GPL双重许可证
MODULE_AUTHOR("sunplusedu");		//模块作者(可选)
MODULE_DESCRIPTION("used for studing linux drivers");	//模块儿简介(可选)
