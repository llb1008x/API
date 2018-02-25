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
#include <linux/kfifo.h> /* kfifo */

#define GPH2_7  _IO('g',7)
#define GPH2_6	_IO('g',6)
#define GPH2_5	_IO('g',5)
#define GPH2_4	_IO('g',4)

#define DEVICE_NAME "demo_char"

static int major = 0;
static struct class *demo_class = NULL;
static struct device *demo_device = NULL;
static struct workqueue_struct *key_workqueue = NULL;
static struct work_struct key_work;
static struct kfifo key_fifo;
static atomic_t open_flag = ATOMIC_INIT(0);
static spinlock_t key_spinlock;

static int demo_open(struct inode *pinode, struct file *pfile)
{
	printk(KERN_WARNING "%s\n%s\n%d\n",__FILE__,__FUNCTION__,__LINE__);
	printk(KERN_WARNING "%s\n",THIS_MODULE->name);
	printk(KERN_WARNING "%d\n",current->pid);
	printk(KERN_WARNING "%s\n",pfile->f_path.mnt->mnt_devname);

	if(atomic_read(&open_flag) == 0)
		enable_irq(IRQ_EINT(23));
	atomic_inc(&open_flag);
	return 0;
}
static int demo_release(struct inode *pinode, struct file *pfile)
{
	printk(KERN_WARNING "%s\n",__FUNCTION__);
	atomic_dec(&open_flag);
	if(atomic_read(&open_flag) == 0)
		disable_irq(IRQ_EINT(23));
	return 0;
}
static ssize_t demo_read(struct file *pfile, char __user *puser, size_t count, loff_t *poffset)
{
	unsigned char key;
	signed ret = 0;
	unsigned int len = min(count,sizeof(key));

	if(kfifo_is_empty(&key_fifo))
		return -1;
	spin_lock(&key_spinlock);
	ret = kfifo_out(&key_fifo,&key,sizeof(key));
	spin_unlock(&key_spinlock);
	
	if(copy_to_user(puser,(void *)&key,len) != 0)
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
static void key_workhandler(struct work_struct *work)
{
	static unsigned char num = 200;
	printk(KERN_WARNING "%s\n",__FUNCTION__);
	num--;
	if(kfifo_is_full(&key_fifo))
		return;
	spin_lock(&key_spinlock);
	kfifo_in(&key_fifo,&num,sizeof(num));	
	spin_unlock(&key_spinlock);
	printk(KERN_WARNING "exit:  %s\n",__FUNCTION__);

}

irqreturn_t key1_irqhandler(int irq,void *data)
{
	printk(KERN_WARNING "in:    %s\n",__FUNCTION__);
	queue_work(key_workqueue,&key_work);
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
	disable_irq(IRQ_EINT(23));
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
	key_workqueue = create_workqueue("key_workqueue");
	INIT_WORK(&key_work,key_workhandler);
	ret = kfifo_alloc(&key_fifo,128,GFP_KERNEL);
	if(ret != 0)
		goto device_create_fail;
	spin_lock_init(&key_spinlock);	
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
	kfifo_free(&key_fifo);
	destroy_workqueue(key_workqueue);
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

