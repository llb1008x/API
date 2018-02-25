/*=========================================================================
  工程名称：	key4+5_driver
  组成文件：	key4+5_src.c
  功能描述： 	通过中断及微线程实现4+5键盘
  硬件连接：	GPH3 1~0与键盘行相连，GPH2 7~3与键盘列相连
=========================================================================*/

#include <linux/device.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/irq.h>
#include <asm/uaccess.h>
#include <asm/irq.h>
#include <asm/io.h>
#include <mach/gpio.h>
#include <linux/poll.h>
/* S5PV210_GPH3_BASE */
#include <mach/regs-gpio.h>

#include <linux/poll.h>  //poll_wait()

#define DEBUG	//open debug message

#ifdef DEBUG
#define PRINTK(fmt, arg...)		printk(KERN_WARNING fmt, ##arg)
#else
#define PRINTK(fmt, arg...)		printk(KERN_DEBUG fmt, ##arg)
#endif

#define GPH2CON					(unsigned long)(S5PV210_GPH2_BASE+ 0x00)
#define GPH2DAT					(unsigned long)(S5PV210_GPH2_BASE + 0x04)
#define GPH2UP					(unsigned long)(S5PV210_GPH2_BASE + 0x08)
#define GPH2DRV					(unsigned long)(S5PV210_GPH2_BASE+ 0x0C)	

#define GPH3CON					(unsigned long)(S5PV210_GPH3_BASE+ 0x00)
#define GPH3DAT					(unsigned long)(S5PV210_GPH3_BASE + 0x04)
#define GPH3UP					(unsigned long)(S5PV210_GPH3_BASE + 0x08)
#define GPH3DRV					(unsigned long)(S5PV210_GPH3_BASE+ 0x0C)

#define DRIVER_NAME	"key4+5"

#define KEY2_IRQ IRQ_EINT(19)
#define KEY3_IRQ IRQ_EINT(20)
#define KEY4_IRQ IRQ_EINT(21)
#define KEY5_IRQ IRQ_EINT(22)
#define KEY6_IRQ IRQ_EINT(23)

static unsigned char key_num[3][5] = {		
								{0,0,0,0,0},
								{0,8,9,10,11},
								{2,3,4,5,6},
								};
								
static wait_queue_head_t wait_queue;
static int MAJOR_NR = 0;		/* Driver Major Number */
static int MINOR_NR = 0;		//次设备起始号
struct class *my_class;	

static struct semaphore readable; //信息号，阻塞read调用
int Have_read;	//解决读之前，按键使readable变得很大的问题
static int col = 0;
static unsigned char key = 0;

static void keyScan_Handler(unsigned long data);

DECLARE_TASKLET(key_tasklet, keyScan_Handler, (unsigned long)&col);

void s5p_gpiodrv_set_func(int GPCON, int bit, int out)
{
    unsigned long tmp;
	tmp = __raw_readl(GPCON);
	out = !!out;
	tmp = (tmp & ~(0x0F << (bit << 2))) | ((out & 0x0F) << (bit << 2));	//4*
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
	tmp = (tmp & ~(0x03 << (bit << 1))) | (type << (bit << 1));		//2*
	__raw_writel(tmp, GPPUD);
}

void s5p_gpiodrv_set_drv(int GPDRV, int bit, int drv)
{
    unsigned int tmp;
    tmp = __raw_readl(GPDRV);
    drv &= 0x03;
    tmp = (tmp & ~(0x03 << (bit << 1))) | (drv << (bit << 1));		//2*
    __raw_writel(tmp, GPDRV);
}


static int init_IO_Eint(void)			//中断模式
{
	int ret;
	//GPH3  0 1 为输出功能，上拉使能，驱动能力为2级，数据初始化为0
	s5p_gpiodrv_set_func(GPH3CON, 0, 1);
	s5p_gpiodrv_set_func(GPH3CON, 1, 1);

	s5p_gpiodrv_set_drv(GPH3DRV, 0, 2);
	s5p_gpiodrv_set_drv(GPH3DRV, 1, 2);

	s5p_gpiodrv_set_bit(GPH3DAT,0,0);	//DAT
	s5p_gpiodrv_set_bit(GPH3DAT,1,0);
	
	// s5p_gpiodrv_set_func(GPH2CON, 3, 0xf);
	// s5p_gpiodrv_set_func(GPH2CON, 4, 0xf);
	// s5p_gpiodrv_set_func(GPH2CON, 5, 0xf);
	// s5p_gpiodrv_set_func(GPH2CON, 6, 0xf);
	// s5p_gpiodrv_set_func(GPH2CON, 7, 0xf);
	
	s5p_gpiodrv_set_pull(GPH2UP, 3, 2);
	s5p_gpiodrv_set_pull(GPH2UP, 4, 2);
	s5p_gpiodrv_set_pull(GPH2UP, 5, 2);
	s5p_gpiodrv_set_pull(GPH2UP, 6, 2);
	s5p_gpiodrv_set_pull(GPH2UP, 7, 2);
	
	ret = set_irq_type(KEY2_IRQ,IRQ_TYPE_EDGE_FALLING);		//必须重新设置type否则不能连续触发
	if(ret != 0){
		return ret;
	}
	ret = set_irq_type(KEY3_IRQ,IRQ_TYPE_EDGE_FALLING);
	if(ret != 0){
		return ret;
	}
	ret = set_irq_type(KEY4_IRQ,IRQ_TYPE_EDGE_FALLING);
	if(ret != 0){
		return ret;
	}
	ret = set_irq_type(KEY5_IRQ,IRQ_TYPE_EDGE_FALLING);
	if(ret != 0){
		return ret;
	}
	ret = set_irq_type(KEY6_IRQ,IRQ_TYPE_EDGE_FALLING);
	if(ret != 0){
		return ret;
	}

	return 0;
}

static void row_scan_init(void)
{
	
	s5p_gpiodrv_set_func(GPH3CON, 0, 0);
	s5p_gpiodrv_set_func(GPH3CON, 1, 0);
	
	s5p_gpiodrv_set_pull(GPH3UP,0, 2);
	s5p_gpiodrv_set_pull(GPH3UP,1, 2);
		
	s5p_gpiodrv_set_func(GPH2CON, 3, 1);
	s5p_gpiodrv_set_func(GPH2CON, 4, 1);
	s5p_gpiodrv_set_func(GPH2CON, 5, 1);
	s5p_gpiodrv_set_func(GPH2CON, 6, 1);
	s5p_gpiodrv_set_func(GPH2CON, 7, 1);

	s5p_gpiodrv_set_drv(GPH2DRV, 3, 2);
	s5p_gpiodrv_set_drv(GPH2DRV, 4, 2);
	s5p_gpiodrv_set_drv(GPH2DRV, 5, 2);
	s5p_gpiodrv_set_drv(GPH2DRV, 6, 2);
	s5p_gpiodrv_set_drv(GPH2DRV, 7, 2);
	
	
	s5p_gpiodrv_set_bit(GPH2DAT,3,0);
	s5p_gpiodrv_set_bit(GPH2DAT,4,0);
	s5p_gpiodrv_set_bit(GPH2DAT,5,0);
	s5p_gpiodrv_set_bit(GPH2DAT,6,0);
	s5p_gpiodrv_set_bit(GPH2DAT,7,0);
		
	return;
}

static irqreturn_t buttons_interrupt(int irq, void *dev_id)
{
	col = (int)dev_id;
	
	tasklet_schedule(&key_tasklet);
	//printk("in buttons_interrupt col= %d\n",col);
	wake_up_interruptible(&wait_queue);			//唤醒等待队列
	return IRQ_HANDLED;
}


static void keyScan_Handler(unsigned long data)
{
	int ret;
	int tmp;
	unsigned char row,row1;
	unsigned char col;
	// printk(" IN keyScan_Handler \n");
	col = *((int*)data);
	row_scan_init();	//	2 3 4 5 6做为输出
	udelay(10);
	
	#if 0
	tmp = __raw_readl(GPH3DAT);
	#endif
	row = (*(unsigned long *)GPH3DAT)&0x03;		//行扫 
	mdelay(100);
	row1 = (*(unsigned long *)GPH3DAT)&0x03;	
	if(row == row1)
	{
		//printk("row=%d  col=%d  key= %d\n",row,col,key);
		if(row != 0x03){
			key = key_num[row][col];
			if(key){
				if(Have_read)	//解决读之前，按键使readable变得很大的问题
				{
					up(&readable);
					//printk("up success!\n");
					Have_read = 0;
				}
			}
		}
	}	
	ret = init_IO_Eint();		//恢复中断模式
	if(ret != 0){
		PRINTK("in function keyscan_handler,IO port initial failed!\n");
	}
}

/* Driver Operation Functions */
static int keyDriver_open(struct inode *inode, struct file *filp)
{
	int ret = 0;
	return ret;
}

static int keyDriver_release(struct inode *inode, struct file *filp)
{
//	int Minor = MINOR(inode->i_rdev);
	PRINTK("keyDriver release called!\n");
	return 0;
}

static ssize_t keyDriver_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
	int ret = 0;
	Have_read = 1;
	if(down_interruptible(&readable) != 0){
		return -1;
	}
	printk("read success!\n");
	ret = copy_to_user(buf, &key, sizeof(key));
	key = 0;	//
	if(ret != 0){
		PRINTK("in function read,copy_to_user function run failed!\n");
		return ret;
	}
	return sizeof(key);
}

static unsigned int keyDriver_poll(struct file *file, struct poll_table_struct *wait)
{
	/*将当前进程添加到队列列表中，但不挂起该进程*/
	poll_wait(file, &wait_queue, wait);
	/*根据当前的键值返回适当的状态掩码*/
	return key==0 ? 0 : POLLIN | POLLRDNORM;
}

/* Driver Operation structure */
static struct file_operations keyDriver_fops = {
	owner:	THIS_MODULE,
	read:	keyDriver_read,
	open:	keyDriver_open,
	poll:	keyDriver_poll,
	release:	keyDriver_release,
};

/* Module Init & Exit function */
static int __init s3c2440_key_init(void)
{
	int ret;
	/* Module init code */
	PRINTK("keyDriver_init\n");
	
	sema_init(&readable, 1);		//信号量初始化
	init_IO_Eint();					//io引脚配置
	
	init_waitqueue_head(&wait_queue);			//初始化等待队列头
	
	//外部中断申请
	ret = request_irq(KEY2_IRQ,&buttons_interrupt, 0/*IRQF_DISABLED*/,"KEY2", (void *)0);
	PRINTK("\nkeyDriver open int19 %d\n",ret);
	ret = request_irq(KEY3_IRQ,&buttons_interrupt, 0,"KEY3", (void *)1);
	PRINTK("keyDriver_open int20 %d\n",ret);
	ret = request_irq(KEY4_IRQ,&buttons_interrupt, 0,"KEY4", (void *)2);
	PRINTK("keyDriver_open int21 %d\n",ret);
	ret = request_irq(KEY5_IRQ,&buttons_interrupt, 0,"KEY5", (void *)3);
	PRINTK("keyDriver_open int22 %d\n",ret);
	ret = request_irq(KEY6_IRQ,&buttons_interrupt, 0,"KEY6", (void *)4);
	PRINTK("keyDriver_open int23 %d\n",ret);
	
	/* Driver register */
	MAJOR_NR = register_chrdev(MAJOR_NR, DRIVER_NAME, &keyDriver_fops);
	if(MAJOR_NR < 0)
	{
		PRINTK("register char device fail!\n");
		return MAJOR_NR;
	}
	my_class=class_create(THIS_MODULE,"udev_key25_tasklet");
	device_create(my_class,NULL, MKDEV(MAJOR_NR, MINOR_NR), NULL,DRIVER_NAME);
	
	PRINTK("register myDriver OK! Major = %d\n", MAJOR_NR);
	return 0;
}

static void __exit s3c2440_key_exit(void)
{
	/* Module exit code */
	free_irq(KEY2_IRQ, (void *)0);
	free_irq(KEY3_IRQ, (void *)1);
	free_irq(KEY4_IRQ, (void *)2);
	free_irq(KEY5_IRQ, (void *)3);
	free_irq(KEY6_IRQ, (void *)4);
	
	tasklet_kill(&key_tasklet);
	
	/* Driver unregister */
	if(MAJOR_NR > 0)
	{
		unregister_chrdev(MAJOR_NR, DRIVER_NAME);
		device_destroy(my_class,MKDEV(MAJOR_NR, MINOR_NR));
		class_destroy(my_class);
		PRINTK("myModule_exit ok\n");
	}
	return;
}

module_init(s3c2440_key_init);
module_exit(s3c2440_key_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("sunplsuedu");


