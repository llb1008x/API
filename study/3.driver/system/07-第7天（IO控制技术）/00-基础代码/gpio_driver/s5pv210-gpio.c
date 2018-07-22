#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/fcntl.h>
#include <linux/vmalloc.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <asm/io.h>
#include <mach/regs-gpio.h>
#include <mach/gpio.h>
#include <linux/device.h>					/* class_create, device_create */

#include <linux/proc_fs.h>

#include "s5pv210-gpio.h"

#define GPIO_DEV_NAME				"s5pv210-gpio"
#define GPIO_DEV_MAJOR				240


struct gpio_op_t {
    int valid;
	const char *group;
	unsigned long iobase;
	int ionum;
#if 0
	void (*reg_dump)(int);
	unsigned int (*read_dat)(int);
	void (*write_dat)(int, unsigned int);
	void (*set_dir)(int, int, int);
	void (*set_bit)(int, int, int);
	void (*set_pull)(int, int, int);
#endif
};

static struct gpio_op_t gpioOpList[] = {
    [0] = { 0, "A0", (unsigned long)S5PV210_GPA0_BASE, S5PV210_GPIO_A0_NR },   //UART0,1
    [1] = { 0, "A1", (unsigned long)S5PV210_GPA1_BASE, S5PV210_GPIO_A1_NR },   //UART2,3
    [2] = { 0, "B",  (unsigned long)S5PV210_GPB_BASE,  S5PV210_GPIO_B_NR },    //SPI0,1
    [3] = { 0, "C0", (unsigned long)S5PV210_GPC0_BASE, S5PV210_GPIO_C0_NR },   //I2S1
    [4] = { 0, "C1", (unsigned long)S5PV210_GPC1_BASE, S5PV210_GPIO_C1_NR },   //I2S2,SPDIF
    [5] = { 0, "D0", (unsigned long)S5PV210_GPD0_BASE, S5PV210_GPIO_D0_NR },   //PWM0,1,2,3
    [6] = { 0, "D1", (unsigned long)S5PV210_GPD1_BASE, S5PV210_GPIO_D1_NR },   //I2C0,1,2
    [7] = { 0, "E0", (unsigned long)S5PV210_GPE0_BASE, S5PV210_GPIO_E0_NR },   //Camera
    [8] = { 0, "E1", (unsigned long)S5PV210_GPE1_BASE, S5PV210_GPIO_E1_NR },   //Camera
    [9] = { 0, "F0", (unsigned long)S5PV210_GPF0_BASE, S5PV210_GPIO_F0_NR },   //LCD
    [10]= { 0, "F1", (unsigned long)S5PV210_GPF1_BASE, S5PV210_GPIO_F1_NR },   //LCD
    [11]= { 0, "F2", (unsigned long)S5PV210_GPF2_BASE, S5PV210_GPIO_F2_NR },   //LCD
    [12]= { 0, "F3", (unsigned long)S5PV210_GPF3_BASE, S5PV210_GPIO_F3_NR },   //LCD
    [13]= { 0, "G0", (unsigned long)S5PV210_GPG0_BASE, S5PV210_GPIO_G0_NR },   //MMC0
    [14]= { 0, "G1", (unsigned long)S5PV210_GPG1_BASE, S5PV210_GPIO_G1_NR },   //MMC1
    [15]= { 0, "G2", (unsigned long)S5PV210_GPG2_BASE, S5PV210_GPIO_G2_NR },   //MMC2
    [16]= { 0, "G3", (unsigned long)S5PV210_GPG3_BASE, S5PV210_GPIO_G3_NR },   //MMC3
    [17]= { 1, "H0", (unsigned long)S5PV210_GPH0_BASE, S5PV210_GPIO_H0_NR },   //EINT0~7
    [18]= { 1, "H1", (unsigned long)S5PV210_GPH1_BASE, S5PV210_GPIO_H1_NR },   //EINT8~15
    [19]= { 1, "H2", (unsigned long)S5PV210_GPH2_BASE, S5PV210_GPIO_H2_NR },   //EINT16~23
    [20]= { 1, "H3", (unsigned long)S5PV210_GPH3_BASE, S5PV210_GPIO_H3_NR },   //EINT24~31
    [21]= { 0, "I",  (unsigned long)S5PV210_GPI_BASE,  S5PV210_GPIO_I_NR },    //I2S0
    [22]= { 1, "J0", (unsigned long)S5PV210_GPJ0_BASE, S5PV210_GPIO_J0_NR },
    [23]= { 1, "J1", (unsigned long)S5PV210_GPJ1_BASE, S5PV210_GPIO_J1_NR },
    [24]= { 1, "J2", (unsigned long)S5PV210_GPJ2_BASE, S5PV210_GPIO_J2_NR },
    [25]= { 1, "J3", (unsigned long)S5PV210_GPJ3_BASE, S5PV210_GPIO_J3_NR },
    [26]= { 1, "J4", (unsigned long)S5PV210_GPJ4_BASE, S5PV210_GPIO_J4_NR },
};

#define GPCON(x)					(gpioOpList[x].iobase + 0x00)
#define GPDAT(x)					(gpioOpList[x].iobase + 0x04)
#define GPPUD(x)					(gpioOpList[x].iobase + 0x08)
#define GPDRV(x)					(gpioOpList[x].iobase + 0x0C)
//#define GPCONPDN(x)				    (gpioOpList[x].iobase + 0x10)
//#define GPPUDPDN(x)				    (gpioOpList[x].iobase + 0x14)

#define DPRINT(fmt, arg...)			printk(fmt, ##arg)

static const char *bit_string1(unsigned int bin, int bit)
{
	static const char *bitMap[] = { "0", "1" };
	bin >>= bit;
	return bitMap[bin & 0x01];
}
static const char *bit_string2(unsigned int bin, int bit)
{
	static const char *bitMap[] = { "00", "01", "10", "11" };
	bin >>= bit;
	return bitMap[bin & 0x03];
}
static const char *bit_string4(unsigned int bin, int bit)
{
	static const char *bitMap[] = {
		"0000", "0001", "0010", "0011",
		"0100", "0101", "0110", "0111",
		"1000", "1001", "1010", "1011",
		"1100", "1101", "1110", "1111",
	};
	bin >>= bit;
	return bitMap[bin & 0x0F];
}
// gpio group op functions
//  ************************type 0
static void gpio_reg_dump(int group)
{
	unsigned int tmp;
    if(gpioOpList[group].group == NULL)
        return;
	tmp = __raw_readl(GPCON(group));
	DPRINT("CON   [%08X]:%4s|%4s|%4s|%4s|%4s|%4s|%4s|%4s\n", tmp
		, bit_string4(tmp, 28), bit_string4(tmp, 24), bit_string4(tmp, 20), bit_string4(tmp, 16)
		, bit_string4(tmp, 12), bit_string4(tmp, 8 ), bit_string4(tmp, 4 ), bit_string4(tmp, 0 ));
	tmp = __raw_readl(GPDAT(group));
	DPRINT("DAT   [%08X]:%4s|%4s|%4s|%4s|%4s|%4s|%4s|%4s\n", tmp
		, bit_string1(tmp, 7), bit_string1(tmp, 6), bit_string1(tmp, 5), bit_string1(tmp, 4)
		, bit_string1(tmp, 3), bit_string1(tmp, 2), bit_string1(tmp, 1), bit_string1(tmp, 0));
	tmp = __raw_readl(GPPUD(group));
	DPRINT("UP    [%08X]:%4s|%4s|%4s|%4s|%4s|%4s|%4s|%4s\n", tmp
		, bit_string2(tmp, 14), bit_string2(tmp, 12), bit_string2(tmp, 10), bit_string2(tmp, 8)
		, bit_string2(tmp, 6), bit_string2(tmp, 4), bit_string2(tmp, 2), bit_string2(tmp, 0));
	tmp = __raw_readl(GPDRV(group));
	DPRINT("DRV   [%08X]:%4s|%4s|%4s|%4s|%4s|%4s|%4s|%4s\n", tmp
		, bit_string2(tmp, 14), bit_string2(tmp, 12), bit_string2(tmp, 10), bit_string2(tmp, 8)
		, bit_string2(tmp, 6), bit_string2(tmp, 4), bit_string2(tmp, 2), bit_string2(tmp, 0));
#if 0
	tmp = __raw_readl(GPCONPDN(group));
	DPRINT("CONPDN[%08X]:%4s|%4s|%4s|%4s|%4s|%4s|%4s|%4s\n", tmp
		, bit_string2(tmp, 14), bit_string2(tmp, 12), bit_string2(tmp, 10), bit_string2(tmp, 8)
		, bit_string2(tmp, 6), bit_string2(tmp, 4), bit_string2(tmp, 2), bit_string2(tmp, 0));
	tmp = __raw_readl(GPPUDPDN(group));
	DPRINT("PUDPDN[%08X]:%4s|%4s|%4s|%4s|%4s|%4s|%4s|%4s\n", tmp
		, bit_string2(tmp, 14), bit_string2(tmp, 12), bit_string2(tmp, 10), bit_string2(tmp, 8)
		, bit_string2(tmp, 6), bit_string2(tmp, 4), bit_string2(tmp, 2), bit_string2(tmp, 0));
#endif
}

unsigned long s5p_gpiodrv_read_dat(int group)
{
    if(gpioOpList[group].group == NULL)
        return 0;
	return __raw_readl(GPDAT(group));
}
EXPORT_SYMBOL_GPL(s5p_gpiodrv_read_dat);

void s5p_gpiodrv_write_dat(int group, unsigned long val)
{
    if(gpioOpList[group].group == NULL)
        return;
	__raw_writel(val, GPDAT(group));
}
EXPORT_SYMBOL_GPL(s5p_gpiodrv_write_dat);

void s5p_gpiodrv_set_func(int group, int bit, int out)
{
    unsigned long tmp;
    if(gpioOpList[group].group == NULL)
        return;
	tmp = __raw_readl(GPCON(group));
	out = !!out;
	tmp = (tmp & ~(0x0F << (bit << 2))) | ((out & 0x0F) << (bit << 2));
	__raw_writel(tmp, GPCON(group));
}
EXPORT_SYMBOL_GPL(s5p_gpiodrv_set_func);

void s5p_gpiodrv_set_bit(int group, int bit, int val)
{
    unsigned int tmp;
    if(gpioOpList[group].group == NULL)
        return;
	tmp = __raw_readl(GPDAT(group));
	val = !!val;
	tmp = (tmp & ~(1 << bit)) | (val << bit);
	__raw_writel(tmp, GPDAT(group));
}
EXPORT_SYMBOL_GPL(s5p_gpiodrv_set_bit);

//type: 0 - disabled, 1 - pull down, 2 - pull up
void s5p_gpiodrv_set_pull(int group, int bit, int type)
{
    unsigned int tmp;
    if(gpioOpList[group].group == NULL)
        return;
	tmp = __raw_readl(GPPUD(group));
	type &= 0x03;
	tmp = (tmp & ~(0x03 << (bit << 1))) | (type << (bit << 1));
	__raw_writel(tmp, GPPUD(group));
}
EXPORT_SYMBOL_GPL(s5p_gpiodrv_set_pull);

void s5p_gpiodrv_set_drv(int group, int bit, int drv)
{
    unsigned int tmp;
    if(gpioOpList[group].group == NULL)
        return;
    tmp = __raw_readl(GPDRV(group));
    drv &= 0x03;
    tmp = (tmp & ~(0x03 << (bit << 1))) | (drv << (bit << 1));
    __raw_writel(tmp, GPDRV(group));
}
EXPORT_SYMBOL_GPL(s5p_gpiodrv_set_drv);

#define MAX_GPOUP_NUMBER	        (sizeof(gpioOpList) / sizeof(gpioOpList[0]))
#define MIN_GPIO					0
#define MAX_GPIO					MAX_GPOUP_NUMBER - 1

static struct proc_dir_entry *gpioEntry = NULL;
static volatile int debug_mode = 0;
int gpio_open(struct inode *inode, struct file *filp)
{
	int Minor = MINOR(inode->i_rdev);
	if((Minor < 0) || (Minor >= MAX_GPOUP_NUMBER))
		return -EINVAL;
    if(gpioOpList[Minor].valid == 0)
        return -EACCES;

	filp->private_data = (void *)Minor;
	if(debug_mode)
	{
//		unsigned int tmp;
        gpio_reg_dump(Minor);
	}
	return 0;
}

int gpio_close(struct inode *inode, struct file *filp)
{
	int Minor = MINOR(inode->i_rdev);
	if((Minor < 0) || (Minor >= MAX_GPOUP_NUMBER))
		return -EINVAL;
	if(debug_mode)
	{
//		unsigned int tmp;
        gpio_reg_dump(Minor);
	}
	return 0;
}

ssize_t gpio_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	int Minor = (int)filp->private_data;
	unsigned long val = 0;
	if(count < sizeof(val))
		return -EINVAL;
	if(debug_mode)
	{
		if(*f_pos)
		return 0;
	}
    val = s5p_gpiodrv_read_dat(Minor);
	copy_to_user(buf, &val, sizeof(val));
	if(debug_mode)
    	*f_pos += sizeof(val);
	return sizeof(val);
}

ssize_t gpio_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
	unsigned long val = 0;
	int Minor = (int)filp->private_data;
	if(debug_mode)
	{
		if(*f_pos)
		return 0;
	}
	if(count > sizeof(val))
		count = sizeof(val);
		
	copy_from_user(&val, buf, count);
	
	s5p_gpiodrv_write_dat(Minor, val);
	
	if(debug_mode)
    	*f_pos += sizeof(val);
	return sizeof(val);
}

int gpio_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
	int Minor = (int)filp->private_data;
//	u32 tmp;
	switch(cmd)
	{
	case GPIO_SET_PIN_IN:
	    s5p_gpiodrv_set_func(Minor, arg, 0);
		break;
	case GPIO_SET_PIN_OUT:
		s5p_gpiodrv_set_func(Minor, arg, 1);
		break;
	case GPIO_CLR_PIN:
		s5p_gpiodrv_set_bit(Minor, arg, 0);
		break;
	case GPIO_SET_PIN:
		s5p_gpiodrv_set_bit(Minor, arg, 1);
		break;
	case GPIO_SET_PULL_DIS:
		s5p_gpiodrv_set_pull(Minor, arg, 0);
		break;
	case GPIO_SET_PULL_DOWN:
		s5p_gpiodrv_set_pull(Minor, arg, 1);
		break;
	case GPIO_SET_PULL_UP:
		s5p_gpiodrv_set_pull(Minor, arg, 2);
		break;
	case GPIO_SET_PIN_DRV0:
	    s5p_gpiodrv_set_drv(Minor, arg, 0);
	    break;
	case GPIO_SET_PIN_DRV1:
	    s5p_gpiodrv_set_drv(Minor, arg, 1);
	    break;
	case GPIO_SET_PIN_DRV2:
	    s5p_gpiodrv_set_drv(Minor, arg, 2);
	    break;
	default:
		return -EINVAL;
	}
	return 0;
}

struct file_operations gpio_fops = {
	.owner = THIS_MODULE,
	.open = gpio_open,
	.release = gpio_close,
	.read = gpio_read,
	.write = gpio_write,
	.ioctl = gpio_ioctl,
};

static int gpio_read_proc(char *page, char **start, off_t offset, int count, int *eof, void *data)
{
	int len = sprintf(page, "%d\n", debug_mode);
	*eof = 1;
	return len;
}

static int gpio_write_proc(struct file *file, const char __user *buffer, unsigned long count, void *data)
{
	char tmp[100];
	int dbg = 0;
	int len = (count > sizeof(tmp)) ? sizeof(tmp) : count;
	copy_from_user(tmp, buffer, len);
	if(sscanf(tmp, "%d", &dbg) == 1)
		debug_mode = dbg;
	return len;
}

static struct class *gpioClass;
static struct device *gpioDevice[MAX_GPOUP_NUMBER];
int gpio_init(void)
{
	int result;
	int i;
	result = register_chrdev(GPIO_DEV_MAJOR, GPIO_DEV_NAME, &gpio_fops);
	
	if(result < 0)
		return result;
		
	gpioEntry = create_proc_entry("gpio_debug", 0, NULL);
	
	gpioEntry->read_proc = &gpio_read_proc;
	gpioEntry->write_proc = &gpio_write_proc;
	
	gpioClass = class_create(THIS_MODULE, "s5pv-gpio");
	for(i = MIN_GPIO; i <= MAX_GPIO; i++)
	{
        if(gpioOpList[i].valid == 0)
            continue;
		gpioDevice[i - MIN_GPIO] = device_create(gpioClass, NULL, MKDEV(GPIO_DEV_MAJOR, i), NULL,"gp%s", gpioOpList[i].group);
	}
	return 0;
}

void gpio_exit(void)
{
	int i;
	remove_proc_entry("gpio_debug", NULL);
	for(i = MIN_GPIO; i <= MAX_GPIO; i++)
		device_destroy(gpioClass, MKDEV(GPIO_DEV_MAJOR, i));
	class_destroy(gpioClass);
	unregister_chrdev(GPIO_DEV_MAJOR, GPIO_DEV_NAME);
	return;
}

module_init(gpio_init);
module_exit(gpio_exit);
MODULE_LICENSE("Dual BSD/GPL");
