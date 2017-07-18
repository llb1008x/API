/*******************************************************************************
 * Filename:
 * ---------
 *  pi5usb30216.c
 *
 * Project:
 * --------
 *
 * Description:
 * ------------
 *
 * Author:
 * -------
 *  Yanjun, yanjun@gionee.com, 2015-07-03
 * 
 *******************************************************************************/
#include <linux/module.h>
#include <linux/delay.h>
#include <linux/i2c.h>
#include <linux/gpio.h>
#include <linux/sched.h>
#include <linux/kthread.h>
#include <linux/bitops.h>
#include <linux/kernel.h>
#include "cust_gpio_usage.h" 
#include <cust_eint.h>
#include <mach/mt_gpio.h>
#include <mach/mt_typedefs.h>
#include <mach/board.h>
#include <mach/eint.h>
#include "pi5usb30216.h"

#define BUF_LEN 16
#define REG_COUNT 4

static int int_flag = 0;
static const struct i2c_device_id pi5usb_i2c_id[] = {{PI5USB_NAME, 0}, {}};
static unsigned short force[] = {0, PI5USB_ADDR, I2C_CLIENT_END, I2C_CLIENT_END};
static const unsigned short *const forces[] = { force, NULL };
static struct task_struct *thread = NULL;
static DECLARE_WAIT_QUEUE_HEAD(waiter);
#ifdef CONFIG_MTK_FAN5405_SUPPORT
extern bool mt_disable_otgcharger(void);
#endif
//Gionee <GN_BSP_USB> <lilubao> <20170316> add for OTG SWITCH begin
#define GN_TYPEC_OTG_SWITCH
#if defined(GN_TYPEC_OTG_SWITCH)
static struct i2c_client *client_global = NULL;
#endif
//Gionee <GN_BSP_USB> <lilubao> <20170316> add for OTG SWITCH end
static int suspend_flag = 0;
/**********************************************************
  *
  *   [I2C Function For Read/Write ] 
  *
  *********************************************************/
int pi5usb_read(struct i2c_client *client, char *val_buf, int count)
{
	int      ret=0;
	
	if(NULL == client || NULL == val_buf || count > REG_COUNT || suspend_flag)
		return -1;
	ret = i2c_master_recv(client, val_buf, count);
	if (ret < 0)
    	{
		printk("%s read failed \n", __func__);
        	return -1;
    	}
#if 1
	printk("ALEX %s: read \n", __func__);
	for(ret  = 0; ret < count; ret++)
		printk("ALEX 0x%x ", val_buf[ret]);
	printk("\n");
#endif
    	return 0;
}
//Only for write reg 0x2
int pi5usb_write(struct i2c_client *client, int writeData)
{
	char    write_data[2] = {0};
	int     ret=0;

	if(NULL == client || suspend_flag)
		return -1;
    	write_data[0] = 0;
    	write_data[1] = (char)(writeData & 0xFF);

    	ret = i2c_master_send(client, write_data, 2);
    	if (ret < 0)
    	{
		printk("ALEX %s write failed \n", __func__);
        	return -1;
    	}
printk("ALEX write OK \n");
    	return 0;
}

static void  pi5usb_reset_i2c(struct i2c_client *client)
{
	if(NULL == client)
	{
		printk("ALEX %s client is null \n", __func__);
		return ;
	}
	pi5usb_write(client, 1);
	msleep(30);
	pi5usb_write(client, 4);
	return ;
}

static int def_mode = 0x4;
static int current_mode = 0x4;
//When attached to a phone/notebook/tablet, our phone as a device
static void pi5usb_try_snk(struct i2c_client *client)
{
	char reg_val[REG_COUNT] = "";
	if(NULL == client)
		return ;
	{
		//TrySNK for 700ms
		current_mode = 0;
		pi5usb_write(client, current_mode + 0x1);
		//TrySNK wait for VBUS
		msleep(700);
		pi5usb_read(client, reg_val, REG_COUNT);
		//if not attached.goto unattached SRC.
		if(0x0 == reg_val[3])
		{
			current_mode = 0x2;
			pi5usb_write(client, current_mode + 0x1);
			msleep(500);
			pi5usb_read(client, reg_val, REG_COUNT);
			if(0x5 == reg_val[3] || 0x6 == reg_val[3])
			{
				printk("ALEX %s: As a host \n", __func__);
			}
			else if(0 == reg_val[3])
			{
				current_mode = 0;
				pi5usb_write(client, current_mode + 0x1);
				msleep(500);
				pi5usb_read(client, reg_val, REG_COUNT);
				//Host attached
				if(0xa9 == reg_val[3]
	                           || 0xaa == reg_val[3]
        	                   || 0xc9 == reg_val[3]
                	           || 0xca == reg_val[3]
                        	   || 0xe9 == reg_val[3]
				   || 0xea == reg_val[3]	
                         	)
				{
#ifdef CONFIG_MTK_FAN5405_SUPPORT
                	                mt_disable_otgcharger();
#endif
	                                printk("ALEX %s: As a devices \n", __func__);
				}
				else	
					current_mode = def_mode;
			}
			else
				current_mode = def_mode;
		}//end tryWait SRC
		else
		{
#ifdef CONFIG_MTK_FAN5405_SUPPORT
                	 mt_disable_otgcharger();
#endif		
			printk("ALEX Try.SNK success. Attached to a host, as a device \n");
		}
	}//end trySNK flow
	return ;
}

static void pi5usb_process(void *data)
{
	struct i2c_client *client = (struct i2c_client *)data;
	char reg_val[REG_COUNT] = "";
	int i = 0;
	static int audio_accessory_flag = 0;
	static int debug_accessory_flag = 0;
	static int repeattimes = 0;
	static int otg_inserted = 0;

	if(NULL == client)
	{
		printk("ALEX %s : client point is null \n", __func__);
		return ;
	}

	pi5usb_write(client, current_mode + 0x1);//Disable interrupt
	msleep(30);//Delay 20ms ~ 45ms
	pi5usb_read(client, reg_val, REG_COUNT);
	if(0x0 == reg_val[3] || (0x80 == reg_val[3]) || 0x1 == reg_val[3])
	{
		current_mode = def_mode;
	}
	switch (reg_val[2])
	{
		case 0x1:
		{
			switch (reg_val[3])
			{
				case 0:
					repeattimes = 0;
					audio_accessory_flag = 0;
					debug_accessory_flag = 0;
					printk("ALEX No plugin \n");
					break;
				case 0x5:
				case 0x6:
					{
						repeattimes = 0;
						otg_inserted = 1;
						pi5usb_try_snk(client);
						//Device Plug in
						printk("ALEX OTG Device plug in \n");
					}
					break;
				case 0x13:
					{
						repeattimes = 0;
						msleep(50);
						debug_accessory_flag = 1; ////确认为Debug accessory且无外部VBUS供电，开启VBUS供电
						printk("ALEX DEBUG Device plug in \n");
					}
					break;
				case 0xa8:
					break;
				case 0x0f:
					//Audio accessory plug in
					audio_accessory_flag = 1;
					msleep(50);
					break;
				case 0x93:
					break;
				case 0x8f:
					break;
				case 0xa9:
				case 0xaa:
				case 0xc9:
				case 0xca:
				case 0xe9:
				case 0xea:
					printk("ALEX Host plug in \n");
					break;
				default:
					break;
			}
		}	
			break;
		case 0x0:
			switch (reg_val[3])
			{
				case 0x0:
					repeattimes = 0;
					audio_accessory_flag = 0;
					debug_accessory_flag = 0;
					break;
				case 0x97:
					if(1 == audio_accessory_flag || 1 == debug_accessory_flag)
					{
						msleep(100);
					}else if(repeattimes >= 3)
					{
						repeattimes = 0;
						printk("ALEX VBUS in, no CC \n");
					}
					else
					{
						repeattimes++;
						msleep(240);
						pi5usb_write(client, 0x1);
						msleep(100);
				//		pi5usb_write(client, 0x4);
					}
					break;
				case 0x93:
					break;
				case 0x8f:
					break;
				case 0xa9:
				case 0xaa:
				case 0xc9:
				case 0xca:
				case 0xe9:
				case 0xea:
					printk("ALEX Host plug in \n");
					break;
				case 0x4:
				case 0x5:
				case 0x6:
				default:
					printk("ALEX %s %d \n", __func__, __LINE__);
					break;

			}
			break;
		case 0x02:
			audio_accessory_flag = 0;
			debug_accessory_flag = 0;
			repeattimes = 0;
			printk("ALEX Detach \n");
			break;
		default:	
			audio_accessory_flag = 0;
			debug_accessory_flag = 0;
			repeattimes = 0;
			break;
	}
	if(0x4 == reg_val[3])
	{
		pi5usb_reset_i2c(client);
	}else
	{
		msleep(20);
		pi5usb_write(client, current_mode);
	}
	return;
}

static void pi5usb_eint_interrupt_handler(void)
{
	int_flag = 1;
	wake_up_interruptible(&waiter);
}
static int pi5usb_event_handler(void *data)
{
	do
    	{
        	set_current_state(TASK_INTERRUPTIBLE);
        	wait_event_interruptible(waiter, int_flag != 0);
        	set_current_state(TASK_RUNNING);
		int_flag = 0; //Must here ,because pi5usb_process may be have interrupts
		//read registers
		pi5usb_process(data);
        }while(!kthread_should_stop());
}

//Gionee <GN_BSP_USB> <lilubao> <20170316> add for OTG SWITCH begin
#define GN_TYPEC_OTG_SWITCH
#if defined(GN_TYPEC_OTG_SWITCH)
int otg_cc_flag = 0;

static ssize_t show_otg_cc_mode(struct device *dev, struct device_attribute *attr, char *buf)
{
	printk(KERN_ERR "%s: show value\n", __func__);
	return sprintf(buf, "%u\n", otg_cc_flag);
}

static ssize_t store_otg_cc_mode(struct device *dev, struct device_attribute *attr,
				       const char *buf, size_t size)
{
	printk(KERN_ERR "%s: store value is %s\n", __func__, buf);

	if(!strncmp("1", buf, 1)){

		printk(KERN_ERR "%s: echo 1 start\n", __func__);		

		//flag to show


		if(suspend_flag != 0){
		
			mt_set_gpio_mode(PI5USB_RESET_PIN, PI5USB_RESET_PIN_MODE);
			mt_set_gpio_dir(PI5USB_RESET_PIN, GPIO_DIR_OUT);
			mt_set_gpio_out(PI5USB_RESET_PIN, 0);
			pi5usb_reset_i2c(client_global);
		
		}
		suspend_flag = 0;
		otg_cc_flag = 1;	

		printk(KERN_ERR "otg_cc_flag->%d,suspend_flag->%d\n",otg_cc_flag,suspend_flag);	
		printk(KERN_ERR "%s: echo 1 end\n", __func__);	

	}else if(!strncmp("0", buf, 1)){

		printk(KERN_ERR "%s: echo 0 start\n", __func__);

		//flag to show
	
		if(suspend_flag == 0){
		
			mt_set_gpio_mode(PI5USB_RESET_PIN, PI5USB_RESET_PIN_MODE);
			mt_set_gpio_dir(PI5USB_RESET_PIN, GPIO_DIR_OUT);
			mt_set_gpio_out(PI5USB_RESET_PIN, 1);
			
		}
		suspend_flag = 1;
		otg_cc_flag = 0;
		
		printk(KERN_ERR "otg_cc_flag->%d,suspend_flag->%d\n",otg_cc_flag,suspend_flag);	
		printk(KERN_ERR "%s: echo 0 end\n", __func__);	
	}

	return size;

}

DEVICE_ATTR(otg_mode, 0664, show_otg_cc_mode, store_otg_cc_mode);
#endif
//Gionee <GN_BSP_USB> <lilubao> <20170316> add for OTG SWITCH end

static s32 pi5usb_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	s32 err = 0;
	s32 ret = 0;
	char reg_val[4] = "";
	//client->timing = 0;	//Reduce i2c speed
	printk("ALEX %s START \n", __func__);
	printk("ALEX PI5USB_INT_PIN %d  \n", PI5USB_INT_PIN);
	printk("ALEX PI5USB_RESET_PIN %d  \n", PI5USB_RESET_PIN);

	mt_set_gpio_mode(PI5USB_INT_PIN, PI5USB_INT_PIN_MODE);
    mt_set_gpio_dir(PI5USB_INT_PIN, GPIO_DIR_IN);
	mt_set_gpio_pull_enable(PI5USB_INT_PIN, GPIO_PULL_ENABLE);
	mt_set_gpio_pull_select(PI5USB_INT_PIN, GPIO_PULL_UP);	

   	mt_eint_registration(PI5USB_INT_NUM, EINTF_TRIGGER_FALLING, pi5usb_eint_interrupt_handler, 1); //Set "1" for disable/enable interrupt auto
   	mt_eint_unmask(PI5USB_INT_NUM);
	
	thread = kthread_run(pi5usb_event_handler, (void *)client, "pi5usb");
	if (IS_ERR(thread))
	{
        	err = PTR_ERR(thread);
        	printk(" failed to create kernel thread: %d\n", err);
    	}

	//Gionee <GN_BSP_USB> <lilubao> <20170316> add for OTG SWITCH begin
	#if defined(GN_TYPEC_OTG_SWITCH)
	//default ,disable OTG function
	mt_set_gpio_mode(PI5USB_RESET_PIN, PI5USB_RESET_PIN_MODE);
	mt_set_gpio_dir(PI5USB_RESET_PIN, GPIO_DIR_OUT);
	mt_set_gpio_out(PI5USB_RESET_PIN, 1);
	suspend_flag = 1;
	
	device_create_file(&client->dev, &dev_attr_otg_mode);
	printk(KERN_ERR "otg_cc_flag->%d,suspend_flag->%d\n",otg_cc_flag,suspend_flag);

	printk(KERN_ERR "%s: disable pi5usb boot process\n", __func__);
	//pinctrl_select_state(pinctrl_usbc, en_output1);

	client_global = client;
	
	#else 
	//reset 
	mt_set_gpio_mode(PI5USB_RESET_PIN, PI5USB_RESET_PIN_MODE);
	mt_set_gpio_dir(PI5USB_RESET_PIN, GPIO_DIR_OUT);
	mt_set_gpio_out(PI5USB_RESET_PIN, 0);
	pi5usb_reset_i2c(client);

	#endif	
	//Gionee <GN_BSP_USB> <lilubao> <20170316> add for OTG SWITCH end

	printk("ALEX %s END\n", __func__);
	return 0;
}
#ifdef CONFIG_PM_SLEEP
static int pi5usb_suspend(struct device *dev)
{
	//Gionee <GN_BSP_USB> <lilubao> <20170316> add for OTG SWITCH begin
	#if defined(GN_TYPEC_OTG_SWITCH)
	
	return 0;	
	#else
	//suspend 
    struct i2c_client *client = to_i2c_client(dev);
	
	mt_set_gpio_mode(PI5USB_RESET_PIN, PI5USB_RESET_PIN_MODE);
	mt_set_gpio_dir(PI5USB_RESET_PIN, GPIO_DIR_OUT);
	mt_set_gpio_out(PI5USB_RESET_PIN, 1);
	suspend_flag = 1;
	#endif
	//Gionee <GN_BSP_USB> <lilubao> <20170316> add for OTG SWITCH end
    return 0;
}

static int pi5usb_resume(struct device *dev)
{
	//Gionee <GN_BSP_USB> <lilubao> <20170316> add for OTG SWITCH begin
	#if defined(GN_TYPEC_OTG_SWITCH)
	return 0;
	#else
	//reset
    struct i2c_client *client = to_i2c_client(dev); 
	mt_set_gpio_mode(PI5USB_RESET_PIN, PI5USB_RESET_PIN_MODE);
	mt_set_gpio_dir(PI5USB_RESET_PIN, GPIO_DIR_OUT);
	mt_set_gpio_out(PI5USB_RESET_PIN, 0);
	pi5usb_reset_i2c(client);
	suspend_flag = 0;
	#endif
	//Gionee <GN_BSP_USB> <lilubao> <20170316> add for OTG SWITCH begin
    return 0;
}
#endif

static SIMPLE_DEV_PM_OPS(pi5usb_pm_ops, pi5usb_suspend, pi5usb_resume);
static struct i2c_board_info __initdata i2c_pi5usb = { I2C_BOARD_INFO(PI5USB_NAME, PI5USB_ADDR)};
static struct i2c_driver pi5usb_i2c_driver =
{

    .probe = pi5usb_i2c_probe,
    .driver = {
                .name   = "pi5usb",
                .owner  = THIS_MODULE,
                .pm     = &pi5usb_pm_ops,
        },
    .id_table = pi5usb_i2c_id,
    .address_list = (const unsigned short *) forces,
};

/* called when loaded into kernel */
static int __init pi5usb_driver_init(void)
{
	int ret = -1;
    	printk("pi5usb30216 driver init\n");
	if (i2c_add_driver(&pi5usb_i2c_driver) != 0)
    	{
		printk("%s i2c add failed \n", __func__);
		return -1;	
	}
   	return 0;
}

/* should never be called */
static void __exit pi5usb_driver_exit(void)
{
	printk("pi5usb driver exit\n");
	i2c_del_driver(&pi5usb_i2c_driver);
	return ;
}
late_initcall(pi5usb_driver_init);
module_exit(pi5usb_driver_exit);
//For register board info early else maybe can't register the i2c device
/* called when loaded into kernel */
static int __init pi5usb_init_i2c_board(void)
{
	int ret = -1;
    	printk("pi5usb30216 init i2c board \n");
    	if((ret = i2c_register_board_info(3, &i2c_pi5usb, 1)) < 0)
		printk("%s: register boardinfo failed ret %d \n", __func__, ret);
   	return 0;
}

/* should never be called */
static void __exit pi5usb_init_i2c_board_exit(void)
{
	printk("pi5usb init i2c board exit\n");
	return ;
}

fs_initcall(pi5usb_init_i2c_board);
module_exit(pi5usb_init_i2c_board_exit);

MODULE_AUTHOR("Jun yan");
MODULE_DESCRIPTION("Gionee TYPEC driver");
MODULE_LICENSE("GPL");
