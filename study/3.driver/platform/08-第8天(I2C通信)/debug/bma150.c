#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/miscdevice.h>
#include <plat/gpio-cfg.h>
#include <mach/gpio.h>

//SDA GPD1_4
//SCL GPD1_5
#define SDA_OUT 	s3c_gpio_cfgpin(S5PV210_GPD1(4),S3C_GPIO_OUTPUT)
#define SDA_IN		s3c_gpio_cfgpin(S5PV210_GPD1(4),S3C_GPIO_INPUT)
#define SCL_OUT		s3c_gpio_cfgpin(S5PV210_GPD1(5),S3C_GPIO_OUTPUT)
#define SCL_IN		s3c_gpio_cfgpin(S5PV210_GPD1(5),S3C_GPIO_INPUT)

#define SDA_SET(v)	gpio_set_value(S5PV210_GPD1(4),(!(!(v))))
#define SDA_GET		gpio_get_value(S5PV210_GPD1(4))
#define SCL_UP		gpio_set_value(S5PV210_GPD1(5),1)
#define SCL_DOWN 	gpio_set_value(S5PV210_GPD1(5),0)

#define I2C_DELAY	mdelay(1)

static void i2c_start(void)
{
	SDA_OUT;
	SCL_OUT;
	SDA_SET(1);
	SCL_UP;
	I2C_DELAY;
	SDA_SET(0);
	I2C_DELAY;
	SCL_DOWN;
	I2C_DELAY;
}

static void i2c_stop(void)
{
	SDA_OUT;
	SCL_OUT;
	SDA_SET(0);
	SCL_DOWN;
	I2C_DELAY;
	SCL_UP;
	I2C_DELAY;
	SDA_SET(1);
	I2C_DELAY;
}

static int i2c_ack_recv(void)
{
	int ret = 0;
	
	SDA_IN;
	SCL_OUT;
	SCL_DOWN;
	I2C_DELAY;
	SCL_UP;
	I2C_DELAY;
	ret = SDA_GET;
	SCL_DOWN;
	I2C_DELAY;

	return ret;
}

static void i2c_ack_send(int val)
{
	SDA_OUT;
	SCL_OUT;
//	SDA_SET(1);
	SCL_DOWN;
	SDA_SET(val);
	I2C_DELAY;
	SCL_UP;
	I2C_DELAY;
	SCL_DOWN;
	I2C_DELAY;
}

static char i2c_byt_recv(void)
{
	char data = 0;
	int i = 0;
	
	SDA_IN;
	SCL_OUT;
	SCL_DOWN;
	I2C_DELAY;
	for(i=0;i<8;i++)
	{
		SCL_UP;
		I2C_DELAY;
		data |= SDA_GET<<(7-i);
		SCL_DOWN;
		I2C_DELAY;
	}

	return data;
}

static void i2c_byt_send(volatile char data)
{
	int i = 0;
	unsigned char zxc = 0;
		
	SDA_OUT;
	SCL_OUT;
	SCL_DOWN;
	I2C_DELAY;
	for(i=0;i<8;i++)
	{
		SDA_SET((data>>(7-i))&1);//SDA_SET((data<<i)>>7);//
	//	printk(KERN_CRIT "%d,%p\n",(zxc*8)/128,*(&zxc+1));
		I2C_DELAY;
		SCL_UP;
		I2C_DELAY;
		SCL_DOWN;
		I2C_DELAY;
	}
}

static void i2c_data_read(char *data,int len,unsigned char chip_addr,unsigned char reg_addr)
{
	int i = 0;
	
	i2c_start();
	i2c_byt_send((chip_addr<<1)|0);
	if(i2c_ack_recv())
	{printk(KERN_CRIT "error 1\n");}
	i2c_byt_send(reg_addr);
	if(i2c_ack_recv())
	{printk(KERN_CRIT "error 2\n");}
	i2c_stop();
	//写时序完成，已经告知从设备，我们要操作某寄存器
	i2c_start();
	i2c_byt_send((chip_addr<<1)|1);
	if(i2c_ack_recv())
	{printk(KERN_CRIT "error 3\n");}
	for(i=0;i<len;i++)
	{
		*(data+i) = i2c_byt_recv();
		i2c_ack_send(i/(len-1));
	}
	i2c_stop();
}

static int bma150_open(struct inode *i_node, struct file *f_ile)
{
	return 0;
}

static ssize_t bma150_read(struct file *f_ile, char __user *buff, size_t len, loff_t *off_t)
{
	char data[10];
	short x = 0,y = 0,z = 0;
	
	i2c_data_read(data,6,0x38,0x02);
//	printk(KERN_CRIT "%d,%d,%d,%d,%d,%d\n",data[0],data[1],data[2],data[3],data[4],data[5]);
	x = (data[0]>>6)|(data[1]<<2);
	y = (data[2]>>6)|(data[3]<<2);
	z = (data[4]>>6)|(data[5]<<2);
	printk(KERN_CRIT "x = %d,y = %d,z = %d\n",x,y,z);
	
	return 5;
}

static int bma150_release(struct inode *i_node, struct file *f_ile)
{
	return 0;
}

static struct file_operations f_ops = {
	.owner = THIS_MODULE,
	.open = bma150_open,
	.read = bma150_read,
	.release = bma150_release,
};

static struct miscdevice bma150_misc = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "bma150_i2c",
	.fops = &f_ops,
};

static int __init my_bma150_init(void)
{
	misc_register(&bma150_misc);
	
	return 0;
}

static void __exit my_bma150_exit(void)
{
	misc_deregister(&bma150_misc);
}

module_init(my_bma150_init);
module_exit(my_bma150_exit);

MODULE_AUTHOR("SUNPLUS");
MODULE_LICENSE("GPL");