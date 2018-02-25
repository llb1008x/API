/*
* Copyright (c) 2014 sunplusedu
* This is a demo, about hwo to create a module
*
* Date and Dition:	2014-04-12	v1.0
* Author:				<xiuhai.deng@sunplusapp.com>
* 
*/

/*
* debug:
*		./t
* 		cat /dev/demo_dev
*		echo "xxx" > /dev/demo_dev
* 		cat xxx.y > /dev/demo_dev
*/

#include <linux/module.h>	/* module_init */
#include <linux/fs.h>	/* file_operations */
#include <linux/device.h>	/* class device */
#include <linux/sched.h>		/* current */
#include <linux/mount.h>		/* struct vfsmount */
#include <asm/io.h>	/* writel() */
#include <plat/gpio-cfg.h>	/* s3c_gpio_cfgpin */
#include <mach/gpio.h>	/* gpio_set_value */
#include <linux/uaccess.h> /* copy_to_user() */
#include <linux/interrupt.h> /* request_irq() */
#include <linux/irq.h> /* IRQ_TYPE_EDGE_FALLING() */
#include <linux/delay.h> /* mdelay() */

#define GPH2_7  _IO('g',7)
#define GPH2_6	_IO('g',6)
#define GPH2_5	_IO('g',5)
#define GPH2_4	_IO('g',4)

#define DEVICE_NAME "demo_char"

static int major = 0;
static struct class *demo_class = NULL;
static struct device *demo_device = NULL;

static int demo_open(struct inode *pinode, struct file *pfile)
{
	printk(KERN_WARNING "%s\n%s\n%d\n",__FILE__,__FUNCTION__,__LINE__);
	printk(KERN_WARNING "%s\n",THIS_MODULE->name);
	printk(KERN_WARNING "%d\n",current->pid);
	printk(KERN_WARNING "%s\n",pfile->f_path.mnt->mnt_devname);
	
	return 0;
}
static int demo_release(struct inode *pinode, struct file *pfile)
{
	printk(KERN_WARNING "%s\n",__FUNCTION__);
	return 0;
}
static ssize_t demo_read(struct file *pfile, char __user *puser, size_t count, loff_t *poffset)
{
	volatile unsigned char data = 0x0;
	unsigned int len = min(count,sizeof(data));
	int num = 300;

	while(num--){
		printk(KERN_WARNING "-----------------------------------------%d\n",num);
		mdelay(20);
	}
	data |= gpio_get_value(S5PV210_GPH2(7));
	data = (data<<1);
	data |= gpio_get_value(S5PV210_GPH2(6));
	data = (data<<1);
	data |= gpio_get_value(S5PV210_GPH2(5));
	data = (data<<1);
	data |= gpio_get_value(S5PV210_GPH2(4));
	
	if(copy_to_user(puser,(void *)&data,len) != 0)
		return -EFAULT;	
	return len;
}
static ssize_t demo_write(struct file *pfile, const char __user *puser, size_t count, loff_t *poffset)
{
	unsigned char data[30] = {0};
	unsigned int len = min(count,sizeof(data));

	printk(KERN_WARNING "%s,%d,%d\n",__FUNCTION__,imajor(pfile->f_dentry->d_inode),iminor(pfile->f_dentry->d_inode));
	printk(KERN_WARNING "count=%d,offset=%d\n",count,(unsigned int)*poffset);

	if(copy_from_user((void *)&data,puser,len) != 0)
		return -EFAULT;	
	printk(KERN_WARNING "write:%s\n",data);
	
	*poffset += len;
	return len;
}
static long demo_unlocked_ioctl(struct file *pfile, unsigned int cmd, unsigned long arg)
{
	printk(KERN_WARNING "%c,%d\n",_IOC_TYPE(cmd),_IOC_NR(cmd));

	switch(cmd){
	case GPH2_7:
		s3c_gpio_cfgpin(S5PV210_GPH2(7), arg);
		s3c_gpio_setpull(S5PV210_GPH2(7),S3C_GPIO_PULL_UP);
		break;
	case GPH2_6:
		s3c_gpio_cfgpin(S5PV210_GPH2(6), arg);
		s3c_gpio_setpull(S5PV210_GPH2(6),S3C_GPIO_PULL_UP);
		break;
	case GPH2_5:
		s3c_gpio_cfgpin(S5PV210_GPH2(5), arg);
		s3c_gpio_setpull(S5PV210_GPH2(5),S3C_GPIO_PULL_UP);
		break;
	case GPH2_4:
		s3c_gpio_cfgpin(S5PV210_GPH2(4), arg);
		s3c_gpio_setpull(S5PV210_GPH2(4),S3C_GPIO_PULL_UP);
		break;
	default:
		break;
	}
	
	return 0;
}

struct file_operations demo_fops = {
.owner = THIS_MODULE,
.open = demo_open,
.read = demo_read,
.write = demo_write,
.unlocked_ioctl = demo_unlocked_ioctl,
.release = demo_release,
};
irqreturn_t key1_irqhandler(int irq,void *data)
{
	int num = 300;
	printk(KERN_WARNING "in:    %s\n",__FUNCTION__);
	while(num--){
		printk(KERN_WARNING "%s:%d\n",__FUNCTION__,num);
		mdelay(20);
	}
	printk(KERN_WARNING "exit:  %s\n",__FUNCTION__);
	return IRQ_HANDLED;
}
irqreturn_t key2_irqhandler(int irq,void *data)
{
	printk(KERN_WARNING "%s\n",__FUNCTION__);
	return IRQ_HANDLED;
}

static int demo_gph_init(void)
{
	signed int ret = 0;
	s3c_gpio_cfgpin(S5PV210_GPH3(0), S3C_GPIO_OUTPUT);
	gpio_set_value(S5PV210_GPH3(0),0);	

	s3c_gpio_setpull(S5PV210_GPH2(7),S3C_GPIO_PULL_UP);
	s3c_gpio_setpull(S5PV210_GPH2(6),S3C_GPIO_PULL_UP);
	
	//ret = request_irq(IRQ_EINT(23),key_irqhandler,IRQ_TYPE_EDGE_BOTH,"demo_key",NULL);
	ret = request_irq(IRQ_EINT(23),key1_irqhandler,IRQ_TYPE_EDGE_FALLING,"demo_key1",NULL);
	if(ret != 0)
		return -1;
	ret = request_irq(IRQ_EINT(22),key2_irqhandler,IRQ_TYPE_EDGE_FALLING,"demo_key2",NULL);
	if(ret != 0)
		return -1;
	
	return 0;
}
static int __init demo_module_init(void)
{
	signed int ret = 0;
	
	printk(KERN_WARNING "%s\n",__FUNCTION__);
	major = register_chrdev(major,DEVICE_NAME,&demo_fops);
	if(major < 0)
	{
		ret = major;
		goto register_chrdev_fail;
	}
	demo_class = class_create(THIS_MODULE,DEVICE_NAME);
	if(IS_ERR(demo_class)){
		ret = PTR_ERR(demo_class);
		goto class_create_fail;
	}
	demo_device = device_create(demo_class,NULL,MKDEV(major,0),NULL,"demo_dev");
	if(IS_ERR(demo_device)){
		ret = PTR_ERR(demo_device);
		goto device_create_fail;
	}
	ret = demo_gph_init();
	
return 0;
device_create_fail:
	class_destroy(demo_class);
class_create_fail:
	unregister_chrdev(major,DEVICE_NAME);
register_chrdev_fail:
	return ret;
}
static void __exit demo_module_exit(void)
{
	printk(KERN_WARNING "%s\n",__FUNCTION__);	
	free_irq(IRQ_EINT(23),NULL);
	free_irq(IRQ_EINT(22),NULL);
	unregister_chrdev(major,DEVICE_NAME);
	device_destroy(demo_class,MKDEV(major,0));
	class_destroy(demo_class);
}

module_init(demo_module_init);
module_exit(demo_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("xiuhai.deng@sunplusapp.com");

