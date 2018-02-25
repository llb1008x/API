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


#include <linux/init.h>		//指定模块儿初始化或清除
#include <linux/module.h>
#include <linux/kernel.h>

/*模块初始化函数，该函数加载到内核时被调用。
  * "__init" 表示该函数仅在初始化时使用，模块装载
  * 完毕该函数会被扔掉(释放所咱内存空间)，因此
  * 不要在初始化后仍要使用的函数上添加此标记
  */
static int __init drvdemo_init(void)
{
	//printk函数的使用，请参看<printk的打印级别.txt>文档
	printk(KERN_WARNING "[msg]:this is a driver demo, in module initial function\n");
	return 0;
}

/*模块清除函数，模块从内核卸载时被调用。
  * "__exit" 表示该函数只能被卸载或系统关闭时调用，
  * 当模块儿直接编译到内核或内核配置为不允许卸
  * 载模块时该函数会被简单的丢弃
  */
static void __exit drvdemo_exit(void)
{
	printk(KERN_WARNING "[msg]:in module exit function\n");
}

module_init(drvdemo_init);
module_exit(drvdemo_exit);

/*为驱动模块指定许可证，以下列出常用的许可证，
 *选其一即可，如果没有显示指定，则默认为专有的
 */
MODULE_LICENSE("GPL"); 	//GNU通用公共许可证
//MODULE_LICENSE("GPL_v2");		//许可证(GPL版本2)
//MODULE_LICENSE("Dual BSD/GPL");	//BSD/GPL双重许可证
//MODULE_LICENSE("Proprietary");	//模块儿为专有的

MODULE_AUTHOR("sunplusedu");		//模块作者(可选)
MODULE_DESCRIPTION("used for study linux drivers");	//模块儿简介(可选)
