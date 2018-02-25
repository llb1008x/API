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

#define	DEMO_DEBUG
#ifdef 	DEMO_DEBUG
#define	dem_dbg(fmt, arg...)		printk(KERN_WARNING fmt, ##arg)
#else
#define	dem_dbg(fmt, arg...)		printk(KERN_DEBUG fmt, ##arg)
#endif

/*记录当前驱动所占用的主设备号*/
static int major = 0;

/*@设备文件open操作处理函数
  *   响应应用程序中对文件的open操作，这个入口可以为NULL,  该函数
  *   成功返回0，失败返回负值，注意不对应open系统调用的返回值
  * @pnode: 表示文件，每个文件都有一个inode结构体与之对应
  * @filp: 代表一个打开的文件，一个文件每打开一次就会在
  *    内核空间生成一个对应的file结构体
  */
static int demo_open (struct inode *pnode, struct file *filp)
{
	/*inode结构体中记录了所打开设备文件的主从设备号，
	  *可通过imajor和iminor函数获取到*/
	dem_dbg("[kern func]: %s  major: %d  minor: %d\n",
					__FUNCTION__, imajor(pnode), iminor(pnode));
	return 0;
}

/*@设备文件read操作处理函数
  *   响应应用程序中对文件的read操作，这个入口为NULL将导致read
  *   系统调用出错并接收到返回值-EINVAL(非法参数)�，该函数返回
  *   非负值表示成功读取的字节数(应用程序read系统调用的返回值)
  * @buf: 用户空间地址(用户传递过来)，指向用户空间缓冲区(保存要
  *    写入的数据或者存放新读入的数据), __user表示该地址为用户空间地址
  * @count: 请求传输的数据长度(用户空间传递过来)
  * @offp: 是一个long long型变量的指针，该变量记录了文件中读写的位置，offp
  *    所指向的变量其实就是fseek移动的文件内部的那个"指针(是个变量)"
  */
static ssize_t demo_read (struct file *filp, char __user *buf, size_t count, loff_t *offp)
{
	unsigned char ary[100] = "you are reading successfully!";
	unsigned long len = min(count, sizeof(ary));	//min是个宏，用来获取两个数中较小的值
	int retval;

	dem_dbg("[kern func]: %s  major: %d  minor: %d\n",
					__FUNCTION__, imajor(filp->f_dentry->d_inode), 
					iminor(filp->f_dentry->d_inode));
	
	/*将ary地址数据拷贝到buf指向位置(内核到用户)，拷贝len个字节
	  *返回值为0表示拷贝成功，返回正数表示拷贝剩余的字节数*/
	if(copy_to_user(buf, ary, len) != 0){
		retval = -EFAULT;
		goto cp_err;
	}

	return len;	//成功返回实际传输的字节数
cp_err:
	return retval;	
}

/*@设备文件write操作处理函数
  *   响应应用程序中对文件的write操作，同read函数
  */
static ssize_t demo_write(struct file *filp, const char __user *buf, size_t count, loff_t *offp)
{
	unsigned char ary[100] = "";
	unsigned long len = min(count, sizeof(ary));	//min是个宏，用来获取两个数中较小的值
	int retval;

	dem_dbg("[kern func]: %s  major: %d  minor: %d\n",
					__FUNCTION__, imajor(filp->f_dentry->d_inode), 
					iminor(filp->f_dentry->d_inode));
	/*将buf地址数据拷贝到ary指向位置(用户到内核)，拷贝len个字节
	  *返回值为0表示拷贝成功，返回正数表示拷贝剩余的字节数*/
	if(copy_from_user(ary, buf, len) != 0){
		retval = -EFAULT;
		goto cp_err;
	}

	printk("[msg]: writing context: %s\n",ary);

	return len;	//成功返回实际传输的字节数
cp_err:
	return retval;	
}

/*@设备文件close操作处理函数
  *   响应应用程序中对文件的close操作，同open函数
  */
static int demo_release (struct inode *pnode, struct file *filp)
{
	dem_dbg("[kern func]: %s  major: %d  minor: %d\n",
					__FUNCTION__, imajor(pnode), iminor(pnode));
	return 0;
}


/*@定义file_operations结构体变量
  * 采用标准C的"标记化结构初始化"语法。该方法允许对
  * 结构体成员进行重新排列，使代码紧凑易读，可移
  * 值性提高
  */
static struct file_operations fops = {
	.owner = THIS_MODULE,	//该宏定义在linux/module.h中，本质是个指针，是指向
							//拥有该结构的模块的指针(当前模块)
	.read = demo_read,
	.write = demo_write,
	.open = demo_open,
	.release = demo_release,
};	

static int __init drvdemo_init(void)
{
	int retval;
	
	dem_dbg("[msg]:this is a driver demo, in module initial function\n");

	/*注册字符驱动函数，成功返回动态分配好的主设备号，失败
	 *返回错误码(负值)*/
	major = register_chrdev(0, "demo_chrdev", &fops);
	if(major < 0){
		retval = major;
		goto chrdev_err;
	}
	return 0;
	
chrdev_err:
	return retval;
}

static void __exit drvdemo_exit(void)
{
	dem_dbg("[msg]:in module exit function\n");
	/*注销字符驱动函数，无返回值，major为已分配的主设备号*/
	unregister_chrdev(major, "demo_chrdev");
}

module_init(drvdemo_init);
module_exit(drvdemo_exit);


MODULE_LICENSE("Dual BSD/GPL");	//BSD/GPL双重许可证

MODULE_AUTHOR("sunplusedu");		//模块作者(可选)
MODULE_DESCRIPTION("used for studing linux drivers");	//模块儿简介(可选)
