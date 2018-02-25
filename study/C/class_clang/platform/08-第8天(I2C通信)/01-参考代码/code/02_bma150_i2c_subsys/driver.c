/*
* Copyright (c) 2014 sunplusedu
* This is a demo, about how to create a module
*
* Date and Dition:	2014-03-12	v1.0
* Author:				<xiuhai.deng@sunplusapp.com>
* 
*/

/*
* debug:
* 		cat xxx > /dev/demo_dev
* 		cat /dev/demo_dev
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
#include <linux/slab.h> /* kzalloc() */
#include <linux/err.h>
#include <asm/delay.h>     /* udelay */
#include <linux/miscdevice.h> /* struct miscdevice */
#include "driverapp.h"
#include <linux/i2c.h>

static struct bma150_data{
	struct i2c_client *client;
	unsigned short chip_addr;
}*pbma150_data;

static int bma150_i2c_master_read(unsigned char *buf,int len)
{
	signed int ret = 0;
	struct i2c_msg msg[2];

	msg[0].addr = pbma150_data->chip_addr;
	msg[0].flags = !I2C_M_RD;
	msg[0].len = 1;	
	msg[0].buf = &buf[0];
	
	msg[1].addr = pbma150_data->chip_addr;
	msg[1].flags = I2C_M_RD;
	msg[1].len = len-1; 
	msg[1].buf = &buf[1];

	ret = i2c_transfer(pbma150_data->client->adapter,msg,2);
	return ret;
}

static int bma150_open(struct inode *pinode, struct file *pfile)
{
	printk(KERN_WARNING "%s\n",__FUNCTION__);	
	return 0;
}
void bma150_get_xyz(struct position *pos)
{
	unsigned char data[7];
	signed short x,y,z;

	memset(data,0,sizeof(data));
	data[0] = BMA150_ACC_X_LSB_REG;
	bma150_i2c_master_read(data,sizeof(data));

	x = (data[1]>>6)|(data[2]<<2);
	y = (data[3]>>6)|(data[4]<<2);
	z = (data[5]>>6)|(data[6]<<2);

	pos->x = (signed short)(x<<6)>>6;
	pos->y = (signed short)(y<<6)>>6;
	pos->z = (signed short)(z<<6)>>6;
}

static ssize_t bma150_read(struct file *pfile, char __user *pbuf, size_t count, loff_t *poff)
{
	int ret = 0;
	struct position pos;
	int len = min(count,sizeof(pos));
	
	bma150_get_xyz(&pos);
	ret = copy_to_user(pbuf,&pos,len);
	return ret;

}
static int bma150_release(struct inode *pinode, struct file *pfile)
{
	printk(KERN_WARNING "%s\n",__FUNCTION__);
	return 0;
}

static struct file_operations bma150_fops = {
.owner = THIS_MODULE,
.open = bma150_open,
.read = bma150_read,
.release = bma150_release,
};
static struct miscdevice bma150_misc = {
.minor = MISC_DYNAMIC_MINOR,
.fops = &bma150_fops,
.name = "bma150"
};
static int __devinit bma150_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	signed int ret;
	unsigned char chip_id[2] = {BMA150_DATA_0_REG,0};
	printk(KERN_WARNING "%s\n",__FUNCTION__);

	if (!i2c_check_functionality(client->adapter, I2C_FUNC_I2C)) {
		ret = -EIO;
		goto i2c_check_fail;
	}
	pbma150_data = kzalloc(sizeof(struct bma150_data),GFP_KERNEL);
	if(IS_ERR(pbma150_data)){
		ret = PTR_ERR(pbma150_data);
		goto kzalloc_fail;
	}
	pbma150_data->client = client;
	pbma150_data->chip_addr = client->addr;
	printk(KERN_WARNING "chip_addr=%#x\n",client->addr);

	i2c_set_clientdata(client,pbma150_data);
	ret = bma150_i2c_master_read(chip_id,sizeof(chip_id));
	printk(KERN_WARNING "ret=%d,chip_ID=%#x\n",ret,chip_id[1]);
	if(ret < 0)
		goto i2c_read_fail;

	ret = misc_register(&bma150_misc);
	if(ret)
		goto i2c_read_fail;
	return 0;
	
i2c_read_fail:
	kfree(pbma150_data);
kzalloc_fail:	
i2c_check_fail:
	return ret;
}
static int __devexit bma150_remove(struct i2c_client *client)
{
	printk(KERN_WARNING "%s\n",__FUNCTION__);

	kfree(pbma150_data);
	return misc_deregister(&bma150_misc);
}
static const struct i2c_device_id bma150_id[] =
{
	{"bma150", 0},
	{},
};

static struct i2c_driver bma150_driver = {
.driver = {
	.owner = THIS_MODULE,
	.name = "bma150",
},
.class = I2C_CLASS_HWMON,
.id_table = bma150_id,
.probe = bma150_probe,
.remove = __devexit_p(bma150_remove),
};

static int __init bma150_init(void)
{
	printk(KERN_WARNING "%s\n",__FUNCTION__);
	return i2c_add_driver(&bma150_driver);
}
static void __exit bma150_exit(void)
{
	printk(KERN_WARNING "%s\n",__FUNCTION__);
	i2c_del_driver(&bma150_driver);
}

module_init(bma150_init);
module_exit(bma150_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("xiuhai.deng@sunplusedu.com");
