#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <asm/uaccess.h>
#include <plat/gpio-cfg.h>
#include <mach/gpio.h>

static unsigned int major = 0;
static struct class *my_class = NULL;

static int demo_open(struct inode *i_node, struct file *f_ile)
{
	printk(KERN_CRIT "zai %s\n",__FUNCTION__);
	
	return 0;
}

static ssize_t demo_read(struct file *f_ile, char __user *buff, size_t len, loff_t *off_t)
{
//	char buf[20] = "my buf in kernel!";
//	printk(KERN_CRIT "zai %s\n",__FUNCTION__);
	//���gph2_3���ŵ�ƽ
	if(gpio_get_value(S5PV210_GPH2(3)))
		printk("key was down!\n");

//	return (strlen(buf)-copy_to_user(buff,buf,strlen(buf)));
	return 0;
}

static int demo_release(struct inode *i_node, struct file *f_ile)
{
	printk(KERN_CRIT "zai %s\n",__FUNCTION__);

	return 0;
}

static int demo_ioctl(struct inode *i_node, struct file *f_ile, unsigned int cmd, unsigned long data)
{
	printk(KERN_CRIT "you say cmd %d\n",cmd);
	switch cmd{
			case 1://���
			case 3://�ص�
		}

	return 0;
}

static struct file_operations f_ops = {
	.owner = THIS_MODULE,
	.open = demo_open,
	.read = demo_read,
	.release = demo_release,
	.ioctl = demo_ioctl,
};

static int __init demo_init(void)
{
	struct device *my_device = NULL;
	printk(KERN_CRIT "zai %s\n",__FUNCTION__);
	major = register_chrdev(0,"my_driver",&f_ops);
	my_class = class_create(THIS_MODULE, "my_class");
	my_device = device_create(my_class,NULL,MKDEV(major,3),NULL,"my_zxc0");
	//1,GPH3(0)����ߵ�ƽ
	s3c_gpio_cfgpin(S5PV210_GPH3(0),S3C_GPIO_OUTPUT);
	gpio_set_value(S5PV210_GPH3(0),1);
	//2,GPH2(3)����״̬�����ң�����
	s3c_gpio_cfgpin(S5PV210_GPH2(3),S3C_GPIO_INPUT);
	s3c_gpio_setpull(S5PV210_GPH2(3),S3C_GPIO_PULL_DOWN);

	return 0;
}

static void __exit demo_exit(void)
{
	printk(KERN_CRIT "zai %s\n",__FUNCTION__);
	unregister_chrdev(major,"my_driver");
	device_destroy(my_class, MKDEV(major,3));
	class_destroy(my_class);
}

module_init(demo_init);
module_exit(demo_exit);

MODULE_AUTHOR("sunplusedu");
MODULE_LICENSE("GPL");