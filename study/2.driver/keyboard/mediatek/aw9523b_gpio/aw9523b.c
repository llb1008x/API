#include <linux/init.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/platform_device.h>
#include <linux/device.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/fs.h>  
#include <linux/kobject.h>
#include <linux/delay.h>
#include <linux/ioctl.h>
#include <linux/input.h>
#include <linux/workqueue.h>
#include <linux/interrupt.h>
#include <linux/i2c.h>
#include <asm/atomic.h>
#include <asm/uaccess.h>
#include <linux/jiffies.h>
#include <linux/gpio.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include "aw9523b.h"
#include <linux/of_irq.h>


#ifdef CONFIG_OF
	int aw9523b_irq;
#endif

//Gionee <gn_by_charging> <lilubao> <20170425> add for change keypad begin
#define AW9523B_DEV_NAME         "aw9523b"
#define AW9523B_I2C_NUM		4		// 3->4
#define AW9523B_I2C_ADDRESS 0x58	// 0xB0 -> 0x58
//Gionee <gn_by_charging> <lilubao> <20170425> add for change keypad end

typedef unsigned char U8;	/* uint8_t*/
//#define I2C_GPIO

struct pinctrl *pinctrl_aw9523b;
struct pinctrl_state *aw9523b_eint,*aw9523b_rst_output0, *aw9523b_rst_output1;
static struct platform_driver aw9523b_pinctrl_driver;

typedef enum {
	P0_0,
	P0_1, 
	P0_2, 
	P0_3, 
	P0_4, 
	P0_5, 
	P0_6, 
	P0_7	
} P0_Enum;
 
typedef enum {
	P1_0, 
	P1_1, 
	P1_2, 
	P1_3, 
	P1_4,	
	P1_5, 
	P1_6, 
	P1_7	
} P1_Enum;
 
typedef enum {
	KEY_STATE_PRESSED,
	KEY_STATE_RELEASED,
	KEY_STATE_LONGPRESS,
	KEY_STATE_REPEATED, 
	KEY_STATE_NULL 
}TOUCHKEY_STATE;

static U8 p0_kbd_used[8] = {
	1,/*P0_0*/
	1,/*P0_1*/
	1,/*P0_2*/
	1,/*P0_3*/
	0,/*P0_4*/
	0,/*P0_5*/
	0,/*P0_6*/
	0,/*P0_7*/
};

static U8 p1_kbd_used[8] = {   
	1,/*P1_0*/
	1,/*P1_1*/
	1,/*P1_2*/
	1,/*P1_3*/
	1,/*P1_4*/
	1,/*P1_5*/
	0,/*P1_6*/
	0,/*P1_7*/ 
};
 
static const P0_Enum COL[X_NUM] = {
	P0_0,
	P0_1,
	P0_2,
	P0_3
};

static const P1_Enum Line[Y_NUM] = {
	P1_0,
	P1_1,
	P1_2,
	P1_3,
	P1_4,
	P1_5
};


//I2C related var
static struct i2c_client * aw9523b_client = NULL;
//static struct platform_driver aw9523b_pdrv;


 
static U8 p0_eint_state   = 0x0;
static U8 p1_eint_state   = 0x0;
static U8 p0_in_out_state = 0x0;
static U8 p1_in_out_state = 0x0;

static U8 p0_kbd_used_temp = 0x0;
static U8 p1_kbd_used_temp = 0x0;

static U8 pre_x = 0x00;
static U8 pre_y = 0x00;

static U8 P0_X[X_NUM];
static U8 P1_Y[Y_NUM];   
 
//static U8 p0_value = 0; 
static U8 p1_value = 0;
 
//static TOUCHKEY_STATE KeyBoardKey_State = KEY_STATE_NULL;
 
static int  aw9523b_chip_id = 0;

static struct input_dev *aw9523b_input_dev;

#define AW9523B_KEY_WWW  253
#define AW9523B_KEY_NULL 0x0
static const unsigned short  key_array[Y_NUM][X_NUM] = {
        {KEY_UP, KEY_MENU, KEY_SEND, KEY_BACK},
        {AW9523B_KEY_NULL, AW9523B_KEY_WWW, KEY_BACKSPACE, KEY_CAMERA},
        {KEY_DOWN, KEY_1, KEY_2, KEY_3},
        {KEY_LEFT, KEY_4, KEY_5, KEY_6},
        {KEY_RIGHT, KEY_7, KEY_8, KEY_9},
        {KEY_REPLY, KEY_SWITCHVIDEOMODE, KEY_0, KEY_KBDILLUMTOGGLE},
};

//static U8 P0_VALUE = 0;
static U8 P1_VALUE = 0;




/* hw reset*/
static void aw9523b_hw_reset(void)
{	 
#ifdef CONFIG_OF
	pinctrl_select_state(pinctrl_aw9523b, aw9523b_rst_output1);
	udelay(50); // >20us reset  old = 200
	pinctrl_select_state(pinctrl_aw9523b, aw9523b_rst_output0);
	udelay(100); // >20us reset  old = 200
	pinctrl_select_state(pinctrl_aw9523b, aw9523b_rst_output1);
	udelay(30);
#else
	mt_set_gpio_mode(AW9523_RESET_PIN, GPIO_MODE_00);
	mt_set_gpio_dir(AW9523_RESET_PIN,GPIO_DIR_OUT);	
	mt_set_gpio_out(AW9523_RESET_PIN,GPIO_OUT_ONE);
	udelay(50); // >20us reset  old = 200
	mt_set_gpio_out(AW9523_RESET_PIN,GPIO_OUT_ZERO);
	udelay(100); // >20us reset  old = 200
	mt_set_gpio_out(AW9523_RESET_PIN,GPIO_OUT_ONE);
	udelay(30); 
#endif 
}





int aw9523b_i2c_send(const struct i2c_client *client, const char *buf, int count)
{
	int ret;
	struct i2c_adapter *adap = client->adapter;
	struct i2c_msg msg;

	msg.addr = client->addr;
	msg.flags = client->flags;
	msg.len = count;
	msg.buf = (char *)buf;
	#ifdef USE_I2C_MTK_EXT
	//msg.timing = client->timing;
	msg.ext_flag = client->ext_flag;
	#endif

	ret = i2c_transfer(adap, &msg, 1);

	/*
	 * If everything went ok (i.e. 1 msg transmitted), return #bytes
	 * transmitted, else error code.
	 */
	return (ret == 1) ? count : ret;
}


int aw9523_i2c_recv(const struct i2c_client *client, char *buf, int count)
{
	struct i2c_adapter *adap = client->adapter;
	struct i2c_msg msg;
	int ret;

	msg.addr = client->addr;
	msg.flags = client->flags;
	msg.flags |= I2C_M_RD;
	msg.len = count;
	msg.buf = buf;
	#ifdef USE_I2C_MTK_EXT
	//msg.timing = client->timing;
	msg.ext_flag = client->ext_flag;
	#endif
	ret = i2c_transfer(adap, &msg, 1);

	/*
	 * If everything went ok (i.e. 1 msg received), return #bytes received,
	 * else error code.
	 */
	return (ret == 1) ? count : ret;
}




static BOOL aw9523b_write_reg(unsigned char addr, unsigned char data)
{
	struct i2c_client *client = aw9523b_client;
	u8 buf[2];
	int ret = 0;

	buf[0] = addr;
	buf[1] = data;

	//aw9523b_i2c_send() returns negative errno, or else the number of bytes written
	ret = aw9523b_i2c_send(client, (const char *)buf, 2);
	if (ret < 0) {
		AW9523_ERR("send command error in write!!\n");
		return 1;
	}
	return 0;
}

static int aw9523b_read_reg(unsigned char addr)
{
	u8 buf;
	int ret = 0;
	struct i2c_client *client = aw9523b_client;

	buf = addr;
	ret = aw9523b_i2c_send(client, (const char *)&buf, 1);
	if (ret < 0) {
		AW9523_ERR("send command error in read!!\n");
		return ret;
	}
	ret = i2c_master_recv(client, (char *)&buf, 1);
	if (ret < 0) {
		AW9523_ERR("reads data error in read!!\n");
		return ret;
	}
	return buf;
}



static BOOL aw9523b_i2c_write_reg(unsigned char reg,unsigned char data)
{
	BOOL ack = 0;
	U8 i;

	//retry times = 50
	for (i = 0; i < AW9523_I2C_MAX_LOOP; i++) {
		ack = aw9523b_write_reg(reg, data);
		if(ack == 0) // ack success
			break;
	}
	return ack;
}


static U8 aw9523b_i2c_read_reg(U8 reg_addr)
{
	U8 i = 0;
	int ret = 0;

	for (i = 0; i < AW9523_I2C_MAX_LOOP; i++) {
		ret = aw9523b_read_reg(reg_addr);

		if(ret >= 0)
			break;
		AW9523_ERR("%s: read error\n",__func__);
	}
	return (U8)ret;
}


static BOOL aw9523b_power_on(void)
{
	BOOL ret = 0;
	unsigned short  count =0;

	aw9523b_hw_reset();  //not cecessary 
 	//retry times, count++ < 120
	while(count++ < 120) {
		if(aw9523b_write_reg(0x55,0x55)) {
			continue;
		}

		if(aw9523b_write_reg(0xaa,0xaa)) {
			continue;
		}   

		if(aw9523b_write_reg(0x55,0xaa)) {
			continue;
		}

		if(aw9523b_write_reg(0xaa,0x55)) {
			continue;
		}
		break;	 
	}

	ret |= aw9523b_write_reg(0x55,0x55); 
	ret |= aw9523b_write_reg(0xaa,0xaa); 
	AW9523_ERR("count = %d, ack = %d\r\n", count, ret);

	return ret;
}


static void aw9523b_p0_p1_in_out_setting(void)
{
	p0_in_out_state = 0xFF;
	aw9523b_i2c_write_reg(0x04, p0_in_out_state);
	p1_in_out_state= 0x0;
	aw9523b_i2c_write_reg(0x05, p1_in_out_state);
}

static void aw9523b_p0_p1_interrupt_setting(void)
{
	p0_eint_state=0x00;
	aw9523b_i2c_write_reg(0x06, p0_eint_state);

	p1_eint_state=0xFF;
	aw9523b_i2c_write_reg(0x07, p1_eint_state);
}

static void aw9523b_p0_int_restore(void)
{
	aw9523b_i2c_write_reg(0x06, p0_eint_state);
}
/*
static void aw9523b_P1_int_restore(void)
{
	aw9523b_i2c_write_reg(0x07, p1_eint_state);
}
static void aw9523b_p1_int_disable(void)
{
	aw9523b_i2c_write_reg(0x07, 0xff);
}
static U8 aw9523b_get_p1(void) 
{
	return aw9523b_i2c_read_reg(0x01);
}
static void aw9523b_set_p0(U8 data)
{
	aw9523b_i2c_write_reg(0x02, data);
}
*/
static void aw9523b_p0_int_disable(void)
{
	aw9523b_i2c_write_reg(0x06, 0xff);
}



static U8 aw9523b_get_p0(void)
{
	return aw9523b_i2c_read_reg(0x00);
}





static void aw9523b_set_p1(U8 data)
{
	aw9523b_i2c_write_reg(0x03, data);
}

static void aw9523b_init(void)
{
	AW9523_FUN(f);

	//hwPowerOn(MT6325_POWER_LDO_VGP3, VOL_2800, "AW9527");
	//hwPowerOn(MT6328_POWER_LDO_VIO18, VOL_2800, "AW9527");

	//Gionee <gn_by_charging> <lilubao> <20170425> add for change keypad begin
	printk("in %s before by lilubao\n", __func__);
	//Gionee <gn_by_charging> <lilubao> <20170425> add for change keypad end

	aw9523b_power_on();
	aw9523b_hw_reset();
	aw9523b_chip_id = aw9523b_i2c_read_reg(0X10);
	if(aw9523b_chip_id == 0x23 ) // read chip_id =0x23h   reg_addr=0x10h
		printk("aw9523b get chip_id success,chip_id = %d\n", aw9523b_chip_id);
	else 
		printk("aw9523b get chip_id failed, error chip_id = %d\n", aw9523b_chip_id);

	aw9523b_p0_int_disable();
	aw9523b_p0_p1_in_out_setting();
	aw9523b_p0_p1_interrupt_setting();
	aw9523b_set_p1(p1_value);

	aw9523b_i2c_read_reg(0x00);
	aw9523b_i2c_read_reg(0x01);

	//Gionee <gn_by_charging> <lilubao> <20170425> add for change keypad begin
	printk("in %s after by lilubao\n", __func__);
	//Gionee <gn_by_charging> <lilubao> <20170425> add for change keypad end
}

static void set_p0_x_p1_y(void)
{
	int i = 0;
	int temp = 0;

	//Gionee <gn_by_charging> <lilubao> <20170425> add for change keypad begin
	printk("in %s before by lilubao\n", __func__);
	//Gionee <gn_by_charging> <lilubao> <20170425> add for change keypad end
	
	for (i = 0;i < X_NUM; i++) {
		temp = temp | (1 << COL[i]);
     	}
	AW9523_LOG("col_temp = %x\r\n", temp);

	for (i = 0; i < X_NUM; i++) {
		P0_X[i] = temp & (~(1 << COL[i]));
		AW9523_LOG("P0_X[%d] = %x\r\n", i, P0_X[i]);
	}

	temp = 0;
	for (i = 0; i < Y_NUM; i++) {
		temp = temp | (1 << Line[i]);
	}
	AW9523_LOG("line_temp = %x\r\n", temp);

	for (i = 0; i < Y_NUM; i++) {
		P1_Y[i] = temp & (~(1 << Line[i]));
		AW9523_LOG("P1_Y[%d] = %x\r\n", i, P1_Y[i]);
	}

	for(i = 0; i < 8; i++) {
		if(p0_kbd_used[i] == 1)
			p0_kbd_used_temp |= 1 << i;
	}
	AW9523_LOG("p0_kbd_used_temp = %x\r\n",p0_kbd_used_temp);  //used for masking data read out

	for(i = 0; i < 8; i++) {
		if(p1_kbd_used[i] == 1)
			p1_kbd_used_temp |= 1 << i;
	}
	AW9523_LOG("p1_kbd_used_temp=%x\r\n",p1_kbd_used_temp);    //0x0f used for masking data read out

	//Gionee <gn_by_charging> <lilubao> <20170425> add for change keypad begin
	printk("in %s after by lilubao\n", __func__);
	//Gionee <gn_by_charging> <lilubao> <20170425> add for change keypad end
}


u16 keyboard_get_press_key(void)
{
	U8 x = 0xFF, y = 0XFF;
	U8 i = 0,j = 0,k = 0;
	U8 get_p0 = 0xff;

	get_p0 = aw9523b_get_p0();
	i = get_p0 | (~(p0_kbd_used_temp));
	AW9523_LOG("%x\r\n", get_p0);

	if (i==0xff)
		return 0xFF;

	for (j = 0; j < X_NUM; j++) {
		if((i & (1 << COL[j])) == 0){
			x = j;
			break;
		}
	}

	if (x == 0xFF) {
	    AW9523_LOG("------get_key = 0xff----\r\n");
		return 0xFF;
	}

	for (j = 0;j < Y_NUM; j++) {
		aw9523b_i2c_write_reg(0x05, P1_Y[j]);
		get_p0 = aw9523b_get_p0(); 
		k = get_p0 | (~(p0_kbd_used_temp));           
		if ((k & (1 << COL[x])) == 0){	
			y = j;
			break;
		}
	}

	aw9523b_i2c_write_reg(0x05, P1_VALUE);
	AW9523_LOG("x = %d, y = %d\r\n", x, y);

	if (x != 0xFF && y!= 0xFF ) {
		pre_x=x;
		pre_y=y;

		return key_array[y][x];
	} else {
		return 0xFF;
	}
} 
static void aw9523b_handler(struct work_struct *work);
static DECLARE_WORK(aw9523b_work, aw9523b_handler);

static u16 linux_keycode = 0xFF;
static u16 pre_linux_keycode = 0xFF;

static int aw9523b_pressed = 0;
static void aw9523b_handler(struct work_struct *work)
{
	aw9523b_p0_int_disable();

	aw9523b_i2c_read_reg(INPUT_PORT0);
    	aw9523b_i2c_read_reg(INPUT_PORT1);

	linux_keycode = keyboard_get_press_key();

	aw9523b_i2c_write_reg(OUTPUT_PORT1, 0);
	aw9523b_p0_int_restore();
#ifdef CONFIG_OF
	enable_irq(aw9523b_irq);
#else
	mt_eint_unmask(CUST_EINT_AW9523_EINT_NUM);
#endif
	if(!aw9523b_pressed) {
		if(linux_keycode == 0xFF){
			return;
			}

		aw9523b_pressed = 1;
		input_report_key(aw9523b_input_dev, linux_keycode, 1);
		input_sync(aw9523b_input_dev);
		pre_linux_keycode = linux_keycode;

		AW9523_LOG("(pressed) keycode = %d\r\n", linux_keycode);
	} else {
		aw9523b_pressed = 0;
		input_report_key(aw9523b_input_dev, pre_linux_keycode, 0);
		input_sync(aw9523b_input_dev);
		AW9523_LOG("(released) keycode = %d\r\n", pre_linux_keycode);
	}
}

void aw9523b_switch(int sel)
{
#ifdef CONFIG_OF
	if(sel)	
		disable_irq_nosync(aw9523b_irq);
	else
		enable_irq(aw9523b_irq);
#else
	if(sel)	
		mt_eint_mask(CUST_EINT_AW9523_EINT_NUM);
	else
		mt_eint_unmask(CUST_EINT_AW9523_EINT_NUM);
#endif
}

static irqreturn_t aw9523b_isr(int irq, void *dev_id)
{
	irqreturn_t ret = IRQ_NONE;
#ifdef CONFIG_OF
	disable_irq_nosync(aw9523b_irq);
	schedule_work(&aw9523b_work);
#else
	mt_eint_mask(CUST_EINT_AW9523_EINT_NUM);
	schedule_work(&aw9523b_work);
#endif
	return ret = IRQ_HANDLED;
}

static ssize_t aw9523b_show_chip_id(struct device_driver *ddri, char *buf)
{
	ssize_t res;

	res = snprintf(buf, PAGE_SIZE, "0x%04X\n", aw9523b_chip_id);

	return res;	
}

static ssize_t aw9523b_show_reg(struct device_driver *ddri, char *buf)
{
    ssize_t res = 0;
    char *ptr = buf;
	
    ptr += sprintf(ptr, "INPUT_PORT0: 0x%x\n", aw9523b_i2c_read_reg(INPUT_PORT0));
    ptr += sprintf(ptr, "INPUT_PORT1: 0x%x\n", aw9523b_i2c_read_reg(INPUT_PORT1));
    ptr += sprintf(ptr, "OUTPUT_PORT0: 0x%x\n", aw9523b_i2c_read_reg(OUTPUT_PORT0));
    ptr += sprintf(ptr, "OUTPUT_PORT1: 0x%x\n", aw9523b_i2c_read_reg(OUTPUT_PORT1));
    ptr += sprintf(ptr, "CONFIG_PORT0: 0x%x\n", aw9523b_i2c_read_reg(CONFIG_PORT0));
    ptr += sprintf(ptr, "CONFIG_PORT1: 0x%x\n", aw9523b_i2c_read_reg(CONFIG_PORT1));
    ptr += sprintf(ptr, "INT_PORT0: 0x%x\n", aw9523b_i2c_read_reg(INT_PORT0));
    ptr += sprintf(ptr, "INT_PORT1: 0x%x\n", aw9523b_i2c_read_reg(INT_PORT1));
    ptr += sprintf(ptr, "IC_ID: 0x%x\n", aw9523b_i2c_read_reg(IC_ID));
    ptr += sprintf(ptr, "CTL: 0x%x\n", aw9523b_i2c_read_reg(CTL));
    ptr += sprintf(ptr, "\n");
    res = ptr - buf;

    return res;
}

static DRIVER_ATTR(aw9523b_reg, S_IRUGO, aw9523b_show_reg, NULL);
static DRIVER_ATTR(aw9523b_chip_id, S_IRUGO, aw9523b_show_chip_id, NULL);

static struct driver_attribute *aw9523b_attr_list[] = {
        &driver_attr_aw9523b_chip_id,
        &driver_attr_aw9523b_reg,
};

static int aw9523b_create_attr(struct device_driver *driver)
{
        int idx, err = 0;
        int num = (int)(sizeof(aw9523b_attr_list)/sizeof(aw9523b_attr_list[0]));

        if (driver == NULL)
                return -EINVAL;

        for(idx = 0; idx < num; idx++) {
                if((err = driver_create_file(driver, aw9523b_attr_list[idx]))) {
                        printk("driver_create_file (%s) = %d\n", aw9523b_attr_list[idx]->attr.name, err);
                        break;
                }
        }

        return err;
}


//static struct platform_driver aw9523b_pdrv;

static int aw9523b_input_event_init(void)
{
	int r;
#ifdef CONFIG_OF
	int ret = 0;
	u32 ints[2] = {0,0};
	struct device_node *node = NULL;
#endif
    AW9523_FUN(f);

	//Gionee <gn_by_charging> <lilubao> <20170425> add for change keypad begin
	printk("in %s before by lilubao\n", __func__);
	//Gionee <gn_by_charging> <lilubao> <20170425> add for change keypad end
	
	aw9523b_input_dev = input_allocate_device();
	if (!aw9523b_input_dev)
		return -ENOMEM;

	aw9523b_input_dev->name = "aw9523b";
	aw9523b_input_dev->id.bustype = BUS_HOST;
	aw9523b_input_dev->id.vendor = 0x0;
	aw9523b_input_dev->id.product = 0x0;
	aw9523b_input_dev->id.version = 0x0;

	__set_bit(EV_KEY, aw9523b_input_dev->evbit);
	
	input_set_capability(aw9523b_input_dev, EV_KEY, KEY_UP);
	input_set_capability(aw9523b_input_dev, EV_KEY, KEY_MENU);
	input_set_capability(aw9523b_input_dev, EV_KEY, KEY_SEND);
	input_set_capability(aw9523b_input_dev, EV_KEY, KEY_BACK);
	input_set_capability(aw9523b_input_dev, EV_KEY, AW9523B_KEY_WWW);
	input_set_capability(aw9523b_input_dev, EV_KEY, KEY_BACKSPACE);
	input_set_capability(aw9523b_input_dev, EV_KEY, KEY_CAMERA);
	input_set_capability(aw9523b_input_dev, EV_KEY, KEY_DOWN);
	input_set_capability(aw9523b_input_dev, EV_KEY, KEY_1);
	input_set_capability(aw9523b_input_dev, EV_KEY, KEY_2);
	input_set_capability(aw9523b_input_dev, EV_KEY, KEY_3);
	input_set_capability(aw9523b_input_dev, EV_KEY, KEY_LEFT);
	input_set_capability(aw9523b_input_dev, EV_KEY, KEY_4);
	input_set_capability(aw9523b_input_dev, EV_KEY, KEY_5);
	input_set_capability(aw9523b_input_dev, EV_KEY, KEY_6);
	input_set_capability(aw9523b_input_dev, EV_KEY, KEY_RIGHT);
	input_set_capability(aw9523b_input_dev, EV_KEY, KEY_7);
	input_set_capability(aw9523b_input_dev, EV_KEY, KEY_8);
	input_set_capability(aw9523b_input_dev, EV_KEY, KEY_9);
	input_set_capability(aw9523b_input_dev, EV_KEY, KEY_REPLY);
	input_set_capability(aw9523b_input_dev, EV_KEY, KEY_SWITCHVIDEOMODE);
	input_set_capability(aw9523b_input_dev, EV_KEY, KEY_0);
	input_set_capability(aw9523b_input_dev, EV_KEY, KEY_KBDILLUMTOGGLE);
	
//	aw9523b_input_dev->dev.parent = &pdev->dev;
	r = input_register_device(aw9523b_input_dev);
	if (r) {
		input_free_device(aw9523b_input_dev);
		return r;
	}

	set_p0_x_p1_y();
	aw9523b_init(); 	//io_init
#ifdef CONFIG_OF
	node = of_find_compatible_node(NULL, NULL, "mediatek,AW9523B-eint"); 
	if(node) {

	    of_property_read_u32_array(node , "debounce", ints, ARRAY_SIZE(ints));
	    gpio_request(ints[0],"flip");
	    gpio_set_debounce(ints[0], ints[1]);

	    aw9523b_irq = irq_of_parse_and_map(node, 0);

	    ret = request_irq(aw9523b_irq, aw9523b_isr, IRQF_TRIGGER_LOW, "AW9523B-eint", NULL);
	    if(ret > 0) 
			printk("%s: request_irq FLIP-eint not available!\n",__func__);
	    else 
			printk("%s: request_irq FLIP-eint finish,hall_irp=%d\n",__func__,aw9523b_irq);

	}
	else{
		printk("%s: of_find_compatible_node FLIP-eint fail!\n",__func__);
	}
	enable_irq(aw9523b_irq);
#else
	AW9523_GPIO_AS_INT(GPIO_AW9523_EN_PIN);
	mt_eint_registration(CUST_EINT_AW9523_EINT_NUM, CUST_EINT_AW9523_EINT_TYPE, aw9523b_isr, 0);
	mt_eint_unmask(CUST_EINT_AW9523_EINT_NUM);
#endif
	/* register device (/dev/mt6575-kpd) */
	//aw9523b_create_attr(&aw9523b_pdrv.driver);
	aw9523b_create_attr(&aw9523b_pinctrl_driver.driver);//wangguojun modify

	return 0;
}




//================== I2C functions ===================//

static int aw9523b_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id);
static int aw9523b_i2c_remove(struct i2c_client *client);

static const struct i2c_device_id aw9523b_i2c_id[] = {{AW9523B_DEV_NAME,0},{}};


static struct i2c_driver aw9523b_i2c_driver = {
    .driver = {
        .owner = THIS_MODULE, 
        .name  = AW9523B_DEV_NAME,
    },
	.probe      = aw9523b_i2c_probe,
	.remove     = aw9523b_i2c_remove,

	.id_table = aw9523b_i2c_id,
};

static int aw9523b_i2c_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	aw9523b_client = client;
	//Gionee <gn_by_charging> <lilubao> <20170425> add for change keypad begin
	printk("in %s before by lilubao\n", __func__);
	//Gionee <gn_by_charging> <lilubao> <20170425> add for change keypad end
	
	//aw9523b_client->timing = 400;
	aw9523b_input_event_init();
 	//LOG_INF("Attached!! \n");

 	//Gionee <gn_by_charging> <lilubao> <20170425> add for change keypad begin
	printk("in %s after by lilubao\n", __func__);
	//Gionee <gn_by_charging> <lilubao> <20170425> add for change keypad end

    return 0;
}


static int aw9523b_i2c_remove(struct i2c_client *client) {
    return 0;
}

/*wangguojun add for aw9523 pinctrl begin */
//aw9523b pin contrl=========================================
static int aw9523b_pinctrl_probe(struct platform_device *pdev){

    int retval = 0;
    #ifdef GN_DEBUG
    printk("%s \n", __func__);
    #endif

	//Gionee <gn_by_charging> <lilubao> <20170425> add for change keypad begin
	printk("in %s before by lilubao\n", __func__);
	//Gionee <gn_by_charging> <lilubao> <20170425> add for change keypad end

    pinctrl_aw9523b = devm_pinctrl_get(&pdev->dev);

    if (IS_ERR(pinctrl_aw9523b)) {
        printk("Cannot find aw9523b pinctrl!\n");
    } else {

        aw9523b_eint = pinctrl_lookup_state(pinctrl_aw9523b, "aw9523b_eint_as_int");
        if (IS_ERR(aw9523b_eint))
            printk("pintest aw9523b_eint error\n");
	#ifdef GN_DEBUG
        else
            printk("pintest aw9523b_eint\n");
	#endif

        aw9523b_rst_output1 = pinctrl_lookup_state(pinctrl_aw9523b, "aw9523b_reset_high");
        if (IS_ERR(aw9523b_rst_output1))
            printk("pintest aw9523b_rst_output high error\n");
	#ifdef GN_DEBUG
        else
            printk("pintest aw9523b_rst_output high\n");
        #endif

        aw9523b_rst_output0 = pinctrl_lookup_state(pinctrl_aw9523b, "aw9523b_reset_low");
        if (IS_ERR( aw9523b_rst_output0))
            printk("pintest aw9523b_rst_output error\n");
        #ifdef GN_DEBUG
        else
            printk("pintest aw9523b_rst_output low\n");
        #endif

    }

	//Gionee <gn_by_charging> <lilubao> <20170425> add for change keypad begin
	printk("in %s after by lilubao\n", __func__);
	//Gionee <gn_by_charging> <lilubao> <20170425> add for change keypad end
	
    return retval;
}
static int aw9523b_pinctrl_remove(struct platform_device *pdev){
	return 0;
}
static const struct of_device_id aw9523b_pinctrl_ids[] = {
	{.compatible = "mediatek,aw9523b_pinctrl",},
	{},
};

static struct platform_driver aw9523b_pinctrl_driver = {
	.probe = aw9523b_pinctrl_probe,
	.remove = aw9523b_pinctrl_remove,
	.driver = {
		.name = "aw9523b_pinctrl",
		.of_match_table = aw9523b_pinctrl_ids,
	},
};
//aw9523b end================================================

/*wangguojun add for aw9523 pinctrl end */
static int __init aw9523b_mod_init(void)
{
	    //register pin ctrl
    if (!platform_driver_register(&aw9523b_pinctrl_driver)){
    	printk("register pinctrl succeed !!!\n");
    }else{
    	printk("register pinctrl fail !!!\n");
        return -1;
    }
	if(i2c_add_driver(&aw9523b_i2c_driver))
	{
		printk(KERN_ERR "add driver error\n");
		return -1;
	} 
	
	return 0;
}

/* should never be called */
static void __exit aw9523b_mod_exit(void)
{
}

module_init(aw9523b_mod_init);
module_exit(aw9523b_mod_exit);

MODULE_AUTHOR("Gionee");
MODULE_DESCRIPTION("Gionee aw9523b Driver v0.0");
