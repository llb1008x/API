
#include <linux/module.h>    /*module_init()*/
#include <linux/kernel.h>	/* printk() */
#include <linux/init.h>		/* __init __exit */
#include <linux/fs.h>	/* file_operation */
#include <asm/uaccess.h>	/* copy_to_user, copy_from_user */
#include <linux/device.h>  /*class ,class_create ,device_create 等*/
#include <linux/errno.h>	/* Error number */
#include <linux/delay.h>	/* udelay */
#include <mach/regs-gpio.h>  /*S3C2410_GPH0CON*/
#include <linux/pci.h>		/*S3C24XX_VA_GPIO*/
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/vmalloc.h>

#include <asm/io.h>
#include <mach/gpio.h>



//#define DEBUG	//open debug message

#ifdef DEBUG
#define PRINTK(fmt, arg...)		printk(KERN_WARNING fmt, ##arg)
#else
#define PRINTK(fmt, arg...)		printk(KERN_DEBUG fmt, ##arg)
#endif

#define DRIVER_NAME	"s5pv210-key15-simple"

#define GPH0CON					(unsigned long)(S5PV210_GPH0_BASE+ 0x00)
#define GPH0DAT					(unsigned long)(S5PV210_GPH0_BASE + 0x04)
#define GPH0UP					(unsigned long)(S5PV210_GPH0_BASE + 0x08)
#define GPH0DRV					(unsigned long)(S5PV210_GPH0_BASE+ 0x0C)

#define GPH2CON					(unsigned long)(S5PV210_GPH2_BASE+ 0x00)
#define GPH2DAT					(unsigned long)(S5PV210_GPH2_BASE + 0x04)
#define GPH2UP					(unsigned long)(S5PV210_GPH2_BASE + 0x08)
#define GPH2DRV					(unsigned long)(S5PV210_GPH2_BASE+ 0x0C)

#define GPH3CON					(unsigned long)(S5PV210_GPH3_BASE+ 0x00)
#define GPH3DAT					(unsigned long)(S5PV210_GPH3_BASE + 0x04)
#define GPH3UP					(unsigned long)(S5PV210_GPH3_BASE + 0x08)
#define GPH3DRV					(unsigned long)(S5PV210_GPH3_BASE+ 0x0C)


static int MAJOR_NR = 0;		/* Driver Major Number */
static int MINOR_NR = 0;		/* Driver Minor Number */
struct class *gpio_class;
static struct device *gpioDevice;

unsigned long s5p_gpiodrv_read_dat(int GPDAT)
{
	return __raw_readl(GPDAT);
}

void s5p_gpiodrv_write_dat(int GPDAT, unsigned long val)
{
	__raw_writel(val, GPDAT);
}


void s5p_gpiodrv_set_func(int GPCON, int bit, int out)
{
    unsigned long tmp;
	tmp = __raw_readl(GPCON);
	out = !!out;
	tmp = (tmp & ~(0x0F << (bit << 2))) | ((out & 0x0F) << (bit << 2));
	__raw_writel(tmp, GPCON);
}


void s5p_gpiodrv_set_bit(int GPDAT, int bit, int val)
{
    unsigned int tmp;
	tmp = __raw_readl(GPDAT);
	val = !!val;
	tmp = (tmp & ~(1 << bit)) | (val << bit);
	__raw_writel(tmp, GPDAT);
}


//type: 0 - disabled, 1 - pull down, 2 - pull up
void s5p_gpiodrv_set_pull(int GPPUD, int bit, int type)
{
    unsigned int tmp;
	tmp = __raw_readl(GPPUD);
	type &= 0x03;
	tmp = (tmp & ~(0x03 << (bit << 1))) | (type << (bit << 1));
	__raw_writel(tmp, GPPUD);
}


void s5p_gpiodrv_set_drv(int GPDRV, int bit, int drv)
{
    unsigned int tmp;
    tmp = __raw_readl(GPDRV);
    drv &= 0x03;
    tmp = (tmp & ~(0x03 << (bit << 1))) | (drv << (bit << 1));
    __raw_writel(tmp, GPDRV);
}


/* Driver Operation Functions */
static int GPH0Driver_open(struct inode *inode, struct file *filp)
{
	//int Minor = MINOR(inode->i_rdev);
	//filp->private_data = 0;
	
	//GPH3  0  为输出功能，上拉使能，驱动能力为2级，数据初始化为0
	s5p_gpiodrv_set_func(GPH3CON, 0, 1);
	s5p_gpiodrv_set_func(GPH3CON, 1, 1);
	
	s5p_gpiodrv_set_pull(GPH3UP, 0, 2);
	s5p_gpiodrv_set_pull(GPH3UP, 1, 2);

	s5p_gpiodrv_set_drv(GPH3DRV, 0, 2);
	s5p_gpiodrv_set_drv(GPH3DRV, 1, 2);

	s5p_gpiodrv_set_bit(GPH3DAT,0,0);
	s5p_gpiodrv_set_bit(GPH3DAT,1,0);
	
	//GPH2  3 4 5 6 7  为输入功能，上拉使能，驱动能力为2级
	s5p_gpiodrv_set_func(GPH2CON, 3, 0);
	s5p_gpiodrv_set_func(GPH2CON, 4, 0);
	s5p_gpiodrv_set_func(GPH2CON, 5, 0);
	s5p_gpiodrv_set_func(GPH2CON, 6, 0);
	s5p_gpiodrv_set_func(GPH2CON, 7, 0);

	s5p_gpiodrv_set_pull(GPH2UP, 3, 2);
	s5p_gpiodrv_set_pull(GPH2UP, 4, 2);
	s5p_gpiodrv_set_pull(GPH2UP, 5, 2);
	s5p_gpiodrv_set_pull(GPH2UP, 6, 2);
	s5p_gpiodrv_set_pull(GPH2UP, 7, 2);

	s5p_gpiodrv_set_drv(GPH2DRV, 3, 2);
	s5p_gpiodrv_set_drv(GPH2DRV, 4, 2);
	s5p_gpiodrv_set_drv(GPH2DRV, 5, 2);
	s5p_gpiodrv_set_drv(GPH2DRV, 6, 2);
	s5p_gpiodrv_set_drv(GPH2DRV, 7, 2);
	
	PRINTK("key15 open called!\n");
	return 0;
}

static int GPH0Driver_release(struct inode *inode, struct file *filp)
{
	//int Minor = MINOR(inode->i_rdev);
	PRINTK("key15 release called!\n");
	return 0;
}
int key_conversion(int key_vlaue)
{
	int key;
	switch(key_vlaue)
	{
		case 0xf0:	//1111 0000
			key=2;
			break;
		case 0xe8:	//1110 1000
			key=3;
			break;
		case 0xd8:	//1101 1000
			key=9;
			break;
		case 0xb8:
			key=10;
			break;
		case 0x78:
			key=11;
			break;
		default:
			key = 0;
			break;
	}
	return key;
}


int key15_scan(void)
{
	int key1=0,key2=0,key=0;
	key1=s5p_gpiodrv_read_dat(GPH2DAT);
	if(key1 != 0xf8)//如果有键按下的可能
	{
		msleep(100);
		key2=s5p_gpiodrv_read_dat(GPH2DAT);
		if(key2!=0xf8)
		{
			if(key1==key2)
			{
				key = key1;
			}
		}
	}
	key = key_conversion(key);
	return key;
}

static ssize_t GPH0Driver_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	int key=0;
	int ret;

	key=key15_scan();
	ret = copy_to_user(buf,&key, sizeof(key));
	return sizeof(key);
}

static ssize_t GPH0Driver_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{

	PRINTK("key15 write called!\n");
	
	return 0;
}

static long GPH0Driver_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	PRINTK("key15 ioctl called(%d)!\n", cmd);
	return 0;
}

/* Driver Operation structure */
static struct file_operations GPH0Driver_fops = {
	owner:		THIS_MODULE,
	write:		GPH0Driver_write,
	read:		GPH0Driver_read,
	unlocked_ioctl:		GPH0Driver_ioctl,
	open:		GPH0Driver_open,
	release:	GPH0Driver_release,
};

/* Module Init & Exit function */
static int __init s5pv210_GPH0_init(void)
{
	/* Module init code */
	PRINTK("s5pv210_GPH0_init\n");
	/* Driver register */
	MAJOR_NR = register_chrdev(MAJOR_NR, DRIVER_NAME, &GPH0Driver_fops);
	if(MAJOR_NR < 0)
	{
		PRINTK("register char device fail!\n");
		return MAJOR_NR;
	}
	gpio_class=class_create(THIS_MODULE,DRIVER_NAME);
	gpioDevice=device_create(gpio_class,NULL, MKDEV(MAJOR_NR, MINOR_NR), NULL,DRIVER_NAME);
	
	PRINTK("register myDriver OK! Major = %d\n", MAJOR_NR);
	return 0;

}

static void __exit s5pv210_GPH0_exit(void)
{
	/* Module exit code */
	PRINTK("s5pv210_GPH0_exit\n");
	/* Driver unregister */
	if(MAJOR_NR > 0)
	{
		unregister_chrdev(MAJOR_NR, DRIVER_NAME);
		device_destroy(gpio_class,MKDEV(MAJOR_NR, MINOR_NR));
		class_destroy(gpio_class);
		PRINTK("myModule_exit ok\n");
	}
	return;
}

module_init(s5pv210_GPH0_init);
module_exit(s5pv210_GPH0_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("sunplusedu");
