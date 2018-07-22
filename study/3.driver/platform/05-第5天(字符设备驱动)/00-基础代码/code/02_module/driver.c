/*
* Copyright (c) 2014 sunplusedu
* This is a demo, about hwo to create a module
*
* Date and Dition:	2014-03-12	v1.0
* Author:				<xiuhai.deng@sunplusapp.com>
* 
*/

#include <linux/module.h>

static int __init demo_module_init(void)
{
	cleanup_module();
	printk(KERN_WARNING "demo_module_init!\n");
	return 0;
}
static void __exit demo_module_exit(void)
{
	printk(KERN_WARNING "demo_module_exit!\n");	
}

module_init(demo_module_init);
module_exit(demo_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("xiuhai.deng@sunplusapp.com");

