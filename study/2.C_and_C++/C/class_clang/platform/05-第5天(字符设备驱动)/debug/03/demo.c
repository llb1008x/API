#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/device.h>

static unsigned int major = 0;
static struct class *my_class = NULL;

static int demo_open(struct inode *i_node, struct file *f_ile)
{
	printk(KERN_CRIT "zai %s\n",__FUNCTION__);
	if(iminor(i_node) == 3)
		printk(KERN_CRIT "zai %s3\n",__FUNCTION__);
	if(iminor(i_node) == 4)
		printk(KERN_CRIT "zai %s4\n",__FUNCTION__);
	
	return 0;
}

static ssize_t demo_read(struct file *f_ile, char __user *buff, size_t len, loff_t *off_t)
{
	printk(KERN_CRIT "zai %s\n",__FUNCTION__);
	if(iminor(f_ile->f_path.dentry->d_inode) == 3)
		printk(KERN_CRIT "zai %s3\n",__FUNCTION__);
	if(iminor(f_ile->f_path.dentry->d_inode) == 4)
		printk(KERN_CRIT "zai %s4\n",__FUNCTION__);

	return 0;
}

static int demo_release(struct inode *i_node, struct file *f_ile)
{
	printk(KERN_CRIT "zai %s\n",__FUNCTION__);

	return 0;
}

static struct file_operations f_ops = {
	.owner = THIS_MODULE,
	.open = demo_open,
	.read = demo_read,
	.release = demo_release,
};

static int __init demo_init(void)
{
	struct device *my_device = NULL;
	printk(KERN_CRIT "zai %s\n",__FUNCTION__);
	major = register_chrdev(0,"my_driver",&f_ops);
	my_class = class_create(THIS_MODULE, "my_class");
	my_device = device_create(my_class,NULL,MKDEV(major,3),NULL,"my_zxc0");
	my_device = device_create(my_class,NULL,MKDEV(major,4),NULL,"my_zxc1");

	return 0;
}

static void __exit demo_exit(void)
{
	printk(KERN_CRIT "zai %s\n",__FUNCTION__);
	unregister_chrdev(major,"my_driver");
	device_destroy(my_class, MKDEV(major,3));
	device_destroy(my_class, MKDEV(major,4));
	class_destroy(my_class);
}

module_init(demo_init);
module_exit(demo_exit);

MODULE_AUTHOR("sunplusedu");
MODULE_LICENSE("GPL");