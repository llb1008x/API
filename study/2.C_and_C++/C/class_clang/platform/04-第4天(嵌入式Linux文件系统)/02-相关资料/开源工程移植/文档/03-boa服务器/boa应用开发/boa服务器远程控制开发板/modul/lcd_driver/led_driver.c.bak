#ifndef __KERNEL__
	#define __KERNEL__
#endif
#ifndef MODULE
	#define MODULE
#endif

#include <asm-arm/arch-s3c2410/S3C2410.h>
#include <linux/config.h>
#include <linux/module.h>
#include <linux/kernel.h>	/* printk() */
#include <linux/init.h>	/* __init __exit */

#include <linux/types.h>	/* size_t */
#include <linux/fs.h>	/* file_operation */
//#include <linux/errno.h>	/* Error number */
//#include <linux/delay.h>	/* udelay */
#include <asm/uaccess.h>	/* copy_to_user, copy_from_user */
#include <asm/hardware.h>

#include "led_driver.h"

#define DEVICE_NAME	"s3c2410_led"

#ifdef DEBUG
#define PRINTK(fmt, arg...)		printk(KERN_NOTICE fmt, ##arg)
#else
#define PRINTK(fmt, arg...)
#endif
/*
	KERN_EMERG		用于紧急事件,一般是系统崩溃前的提示信息
	KERN_ALERT		用于需要立即采取动作的场合
	KERN_CRIT		临界状态,通常设计验证的硬件或软件操作失败
	KERN_ERR		用于报告错误状态.设备驱动程序通常会用它报告来自硬件的问题
	KERN_WARNING	就可能出现的问题提出警告.这些问题通常不会对系统造成严重破坏
	KERN_NOTICE		有必要提示的正常情况.许多安全相关的情况用这个级别汇报
	KERN_INFO		提示性信息.有很多驱动程序在启动时用这个级别打印相关信息
	KERN_DEBUG		用于调试的信息
*/

static int Major = 0;		/* Driver Major Number */
unsigned short InputMask = 0x00;
unsigned short OutputMask = 0xff;
/* Vitual Driver Buffer */
//static unsigned char s3c2410_led_Buffer[1024*1024];

/* Driver Operation Functions */
static int s3c2410_led_open(struct inode *inode, struct file *filp)
{
	MOD_INC_USE_COUNT;
	PRINTK("s3c2410_led open called!\n");
	return 0;
}

static int s3c2410_led_release(struct inode *inode, struct file *filp)
{
	MOD_DEC_USE_COUNT;
	PRINTK("s3c2410_led release called!\n");
	return 0;
}

static ssize_t s3c2410_led_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	unsigned int IOData = read_gpio_reg(GPIO_F0);
	copy_to_user(buf, &IOData, sizeof(IOData));
	PRINTK("read: IOData = 0x%x, count = %d\n", IOData, count);
	return sizeof(IOData);
}

static ssize_t s3c2410_led_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
	unsigned int IOData;
	copy_from_user(&IOData, buf, sizeof(IOData));
	write_gpio_reg(GPIO_F0, IOData);
	PRINTK("write:IOData = 0x%x, count = %d\n", IOData, count);
	return sizeof(IOData);
}

static int s3c2410_led_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
	PRINTK("s3c2410_led ioctl called(%d)!\n", cmd);
    if(_IOC_TYPE(cmd) != GPIO_IOF_MAGIC)
    {
        return -ENOTTY;
    }

    if(_IOC_NR(cmd) >= GPIO_MAXNR)
    {
        return -ENOTTY;
    }
	
	switch(cmd)
	{
		case GPIO_SET_PIN_OUT:
			if(arg < 6)
			{
				OutputMask |= (1 << arg);
				InputMask &= ~(1 << arg);
				set_gpio_ctrl(GPIO_MODE_OUT | GPIO_PULLUP_DIS | MAKE_GPIO_NUM(PORTF_OFS, arg));
			}
			break;
		case GPIO_SET_PIN_IN:
			if(arg < 6)				
			{
				InputMask |= (1 << arg);
				OutputMask &= ~(1 << arg);
				set_gpio_ctrl(GPIO_MODE_IN | GPIO_PULLUP_DIS | MAKE_GPIO_NUM(PORTF_OFS, arg));
			}
			break;
		case GPIO_SET_MULTI_PIN_OUT:
			{
				int i;
				OutputMask |= arg;
				InputMask &= ~(arg);
				for(i = 0; i < 6; i++)
				{
					if(arg & (1 << i))
					{
						PRINTK("arg = %x, setout = %d\n", arg, i);
						set_gpio_ctrl(GPIO_MODE_OUT | GPIO_PULLUP_DIS | MAKE_GPIO_NUM(PORTF_OFS, i));
					}
				}
			}
			break;
		case GPIO_SET_MULTI_PIN_IN:
			{
				int i;
				InputMask |= arg;
				OutputMask &= ~(arg);
				for(i = 0; i < 6; i++)
				{
					if(arg & (1 << i))
						set_gpio_ctrl(GPIO_MODE_IN | GPIO_PULLUP_DIS | MAKE_GPIO_NUM(PORTF_OFS, i));
				}
			}
			break;
	}
	return 0;
}

/* Driver Operation structure */
static struct file_operations s3c2410_fops = {
	owner:		THIS_MODULE,
	write:		s3c2410_led_write,
	read:		s3c2410_led_read,
	ioctl:		s3c2410_led_ioctl,
	open:		s3c2410_led_open,
	release:	s3c2410_led_release,
};

/* Module Init & Exit function */
#ifdef CONFIG_DEVFS_FS
devfs_handle_t devfs_s3c2410_led_dir;
devfs_handle_t devfs_s3c2410_led_raw;
#endif
static int __init s3c2410_led_init(void)
{
	/* Module init code */
	PRINTK("s3c2410_led_init\n");
	set_gpio_ctrl(GPIO_MODE_IN | GPIO_PULLUP_DIS | GPIO_F0);
	set_gpio_ctrl(GPIO_MODE_IN | GPIO_PULLUP_DIS | GPIO_F1);
	set_gpio_ctrl(GPIO_MODE_IN | GPIO_PULLUP_DIS | GPIO_F2);
	set_gpio_ctrl(GPIO_MODE_IN | GPIO_PULLUP_DIS | GPIO_F3);
	set_gpio_ctrl(GPIO_MODE_IN | GPIO_PULLUP_DIS | GPIO_F4);
	set_gpio_ctrl(GPIO_MODE_IN | GPIO_PULLUP_DIS | GPIO_F5);
//	set_gpio_ctrl(GPIO_MODE_IN | GPIO_PULLUP_DIS | GPIO_F6);
//	set_gpio_ctrl(GPIO_MODE_IN | GPIO_PULLUP_DIS | GPIO_F7);
	/* Driver register */
	Major = register_chrdev(0, DEVICE_NAME, &s3c2410_fops);
	if(Major < 0)
	{
		PRINTK("can't get major number\n");
		return Major;
	}
	PRINTK("register s3c2410_led OK! Major = %d\n", Major);
#ifdef CONFIG_DEVFS_FS
	devfs_s3c2410_led_dir = devfs_mk_dir(NULL, "gpf", NULL);
	devfs_s3c2410_led_raw = devfs_register(devfs_s3c2410_led_dir, "0", DEVFS_FL_DEFAULT, Major, 0, S_IFCHR | S_IRUSR | S_IWUSR, &s3c2410_fops, NULL);
	PRINTK("add dev file to devfs OK!\n");
#endif
	return 0;
}

static void __exit s3c2410_led_exit(void)
{
	/* Module exit code */
	PRINTK("s3c2410_led_exit\n");
	/* Driver unregister */
#ifdef CONFIG_DEVFS_FS
	devfs_unregister(devfs_s3c2410_led_raw);
	devfs_unregister(devfs_s3c2410_led_dir);
#endif
	unregister_chrdev(Major, DEVICE_NAME);
}

MODULE_AUTHOR("lijun");
MODULE_LICENSE("Dual BSD/GPL");
module_init(s3c2410_led_init);
module_exit(s3c2410_led_exit);
