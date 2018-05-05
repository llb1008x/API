/*************************************************************************
	> File Name: hellodemo.c
	> Author: 
	> Mail: 
	> Created Time: 2017年09月29日 星期五 08时40分10秒

hello world kernel driver module
 ************************************************************************/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int __init hello_init(void)
{

    printk(KERN_EMERG,"hello world!from the kernel space!\n");
    return 0;
}


static void __exit hello_exit(void)
{

    printk(KERN_EMERG,"goodbye,world!leaving kernel space!\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("lilubao@126.com");
