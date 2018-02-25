#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

static struct input_dev *my_input = NULL;

static int __init my_input_init(void)
{
	my_input = input_allocate_device();


	input_register_device(my_input);
	
	return 0;
}

static void __exit my_input_exit(void)
{
	input_unregister_device(my_input);
}

module_init(my_input_init);
module_exit(my_input_exit);

MODULE_AUTHOR("SUNPLUS");
MODULE_LICENSE("GPL");