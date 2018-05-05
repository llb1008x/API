#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int __init demo_init(void)
{
	printk(KERN_CRIT "zai %s\n",__FUNCTION__);

	return 0;
}

static void __exit demo_exit(void)
{
	printk(KERN_CRIT "zai %s\n",__FUNCTION__);
}

module_init(demo_init);
module_exit(demo_exit);

MODULE_AUTHOR("sunplusedu");
MODULE_LICENSE("GPL");