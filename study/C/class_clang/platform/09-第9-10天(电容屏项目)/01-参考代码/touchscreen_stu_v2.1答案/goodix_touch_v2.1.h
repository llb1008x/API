#ifndef __GOODIX_TOUCH_V2_H__
#define __GOODIX_TOUCH_V2_H__

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/time.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/earlysuspend.h>
#include <linux/hrtimer.h>
#include <linux/i2c.h>
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <mach/gpio.h>
#include <plat/gpio-cfg.h>
#include <linux/irq.h>

#define GOODIX_I2C_NAME "Goodix-TS"
#define TOUCH_MAX_HEIGHT 	7680			
#define TOUCH_MAX_WIDTH	 	5120
#if 0
#define SCREEN_MAX_HEIGHT	480				
#define SCREEN_MAX_WIDTH	272
#else
#define SCREEN_MAX_HEIGHT	800				
#define SCREEN_MAX_WIDTH	480
#endif

#define MAX_FINGER_NUM	5						//最大支持手指数(<=5)
#define FLAG_UP 		0
#define FLAG_DOWN 	1

/* need modify if platform changed */
#define  INT_PORT  	S5P_EXT_INT0(4)			//Int IO port;GPH0CON[4]/EXT_INT[4]
#define  INT_FUNC           gpio_to_irq(INT_PORT)
/* need modify if platform changed */
#define 	SHUTDOWN_PORT S5P_EXT_INT1(3)	//SHUTDOWN管脚号  GPH1CON[3]/EXT_INT[11]  


typedef int (*pos_t)[2];

struct goodix_priv {	
	struct i2c_client *cli;
	struct input_dev *input_dev;
	struct work_struct  work;
	void *privata_data;
};

struct point_node
{
	unsigned int num;
	unsigned int state;
};

struct point_queue
{
	int length;
	struct point_node pointer[MAX_FINGER_NUM];
};

struct point_queue  finger_list;	//record the fingers list 

static void del_point(struct point_queue *point_list)
{
	int count = point_list->length-1;
	int position;
	for(; count >= 0; count--)		//note: must search from tail to head
            if(point_list->pointer[count].state == FLAG_UP)
            	{             	
			if(point_list->length == 0 )
				return ;
			position = count;
			for(; position < MAX_FINGER_NUM -1; position++)
				point_list->pointer[position] = point_list->pointer[position+1];
			point_list->length--;
            	}
}


static int add_point(struct point_queue *point_list, int num)
{
	if(point_list->length >= MAX_FINGER_NUM || num < 0 )
		return -1;
	point_list->pointer[point_list->length].num = num;
	point_list->pointer[point_list->length].state = FLAG_DOWN;
	point_list->length++;
	return 0;
}


static int set_up_point(struct point_queue *point_list, int num)
{
	int count = 0;
	if(point_list->length <= 0 || num < 0 || num > MAX_FINGER_NUM)
		return -1;	//no data
	for(; count < point_list->length; count++)
		if(point_list->pointer[count].num == num)
		{
            		point_list->pointer[count].state = FLAG_UP;
            		return 0;
		}
		else continue;
	return -1;
}


static int goodix_analysis_data(uint8_t *point_data, struct goodix_priv *goodix, pos_t pos, unsigned int *pressure)
{
	static uint8_t  finger_bit=0;
	uint8_t finger=0;
	int position =0;
	int count = 0, read_position = 0;
	int check_sum = 0;
	//The bit indicate which fingers pressed down
	switch(point_data[1]& 0x1f)
	{
		case 0:
		case 1:
			for(count=1; count<8; count++)
				check_sum += (int)point_data[count];
			
			if((check_sum%256) != point_data[8])
				goto XFER_ERROR;
			break;
		case 2:
		case 3:
			for(count=1; count<13;count++)
				check_sum += (int)point_data[count];
			if((check_sum%256) != point_data[13])
				goto XFER_ERROR;
			break;	
		default:		//(point_data[1]& 0x1f) > 3
			for(count=1; count<34;count++)
				check_sum += (int)point_data[count];
			if((check_sum%256) != point_data[34])
				goto XFER_ERROR;
	}
	
	point_data[1]&=0x1f;
	finger = finger_bit^point_data[1];
	if(finger == 0 && point_data[1] == 0)			
		goto NO_ACTION;						//no fingers and no action
		
	else if(finger == 0)							//the same as last time
		goto BIT_NO_CHANGE;					
	
	//check which point(s) DOWN or UP
	for(position = 0; (finger !=0)&& (position < MAX_FINGER_NUM);  position++)
	{
		if((finger&0x01) == 1)		//current bit is 1?
		{							//NO.postion finger is change
		
			if(((finger_bit>>position)&0x01)==1)	//NO.postion finger is UP
				set_up_point(&finger_list, position);
			else 
				add_point(&finger_list, position);
			
		}
		finger>>=1;
	}

BIT_NO_CHANGE:
	for(count = 0; count < finger_list.length; count++)
	{	
		if(finger_list.pointer[count].state == FLAG_UP)
		{
			pos[count][0] = pos[count][1] = 0;	
			pressure[count] = 0;
			continue;
		}
		
		if(finger_list.pointer[count].num < 3)
			read_position = finger_list.pointer[count].num*5 + 3;
		else if (finger_list.pointer[count].num == 4)
			read_position = 29;
		
		if(finger_list.pointer[count].num != 3)
		{
			pos[count][0] = (unsigned int) (point_data[read_position]<<8) + (unsigned int)( point_data[read_position+1]);
			pos[count][1] = (unsigned int)(point_data[read_position+2]<<8) + (unsigned int) (point_data[read_position+3]);
			pressure[count] = (unsigned int) (point_data[read_position+4]);
		}
		else 
		{
			pos[count][0] = (unsigned int) (point_data[18]<<8) + (unsigned int)( point_data[25]);
			pos[count][1] = (unsigned int)(point_data[26]<<8) + (unsigned int) (point_data[27]);
			pressure[count] = (unsigned int) (point_data[28]);
		}
#ifdef GOODIX_TS_DEBUG	
		if(count == 3)
			dev_info(&(goodix->cli->dev), "Original data: Count:%d Index:%d X:%d Y:%d\n",count, finger_list.pointer[count].num,  pos[count][0], pos[count][1]);
#endif
 

		pos[count][0] = (TOUCH_MAX_WIDTH - pos[count][0])*SCREEN_MAX_WIDTH/TOUCH_MAX_WIDTH;//y,
		pos[count][1]=  pos[count][1]*SCREEN_MAX_HEIGHT/TOUCH_MAX_HEIGHT ;					 //x,
		swap(pos[count][0] ,pos[count][1]); 
#ifdef GOODIX_TS_DEBUG
		if(count == 3)
			dev_info(&(goodix->cli->dev), " Coordinate: Count:%d Index:%d X:%d Y:%d\n",count, finger_list.pointer[count].num, pos[count][0], pos[count][1]);
#endif
	}

	return 0;
	
XFER_ERROR:	
NO_ACTION:

	return -1;
}

#endif
