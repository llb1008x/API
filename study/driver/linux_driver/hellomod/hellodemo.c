/*************************************************************************
	> File Name: hellodemo.c
	> Author: 
	> Mail: 
	> Created Time: 2017年09月29日 星期五 08时40分10秒

hello kernel module
 ************************************************************************/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int _init hello_init(void)
{

    printk("<1> hello world!from the kernel space!\n");
    return 0;
}


static void _exit hello_exit(void)
{

    printk("<1> goodbye,world!leaving kernel space!\n");
}

module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
