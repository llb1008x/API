/*
* Copyright (c) 2014 sunplusedu
* This is a demo,about hwo to create a platform device
* 
* Date and Dition:		2014-03-19 	v1.0
* Author:				<xiuhai.deng@sunplusapp.com>
*
*/

#include <linux/module.h>
#include <linux/ioport.h>	/* resource */
#include <linux/irq.h>	/* IRQ_EINT(23) */
#include <linux/gpio.h>	/* S5PV210_GPH3(0) */
#include <linux/platform_device.h>
#include <mach/map.h>
#include <linux/i2c.h>


static struct i2c_client *bma150_client;
static struct i2c_board_info bma150_info = {
	I2C_BOARD_INFO("bma150",0x38),
};

/* Module Init & Exit function */
static int __init bma150_device_init(void)
{
	struct i2c_adapter *i2c_adap;

	i2c_adap = i2c_get_adapter(2);
	bma150_client = i2c_new_device(i2c_adap,&bma150_info);
	i2c_put_adapter(i2c_adap);
	
	return 0;
}

static void __exit bma150_device_exit(void)
{
	i2c_unregister_device(bma150_client);
}

module_init(bma150_device_init);
module_exit(bma150_device_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("xiuhai.deng@sunplusapp.com");


