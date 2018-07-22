/*
 * s3c2410-adc.c
 *
 * S3C2410 ADC 
 *  exclusive with s3c2410-ts.c
 *
 * Author: SeonKon Choi <bushi@mizi.com>
 * Date  : $Date: 2003/01/20 14:24:49 $ 
 *
 * $Revision: 1.1.2.6 $
 *

   Fri Dec 03 2002 SeonKon Choi <bushi@mizi.com>
   - initial

 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file COPYING in the main directory of this archive
 * for more details.
 */
#include <linux/config.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

#include <linux/sched.h>
#include <linux/irq.h>
#include <linux/delay.h>
#include <asm/hardware.h>
#include <asm/semaphore.h>
#include <asm/uaccess.h>

#include "s3c2410-adc.h"
#undef DEBUG
#define DEBUG

#ifdef DEBUG
//#define DPRINTK(x...) {printk(__FUNCTION__"(%d): ",__LINE__);printk(##x);}
#define DPRINTK		printk
#else
#define DPRINTK(x...) (void)(0)
#endif

#define START_ADC_AIN(x) \
	{ \
		ADCCON = PRESCALE_EN | PRSCVL(255) | ADC_INPUT((x)) ; \
		ADCCON |= ADC_START; \
	}

static struct semaphore adc_lock;
static devfs_handle_t devfs_adcraw, devfs_adc_dir;   // ж���豸Ŀ¼
static int Major;
static int Channel;
#define DEVICE_NAME  "s3c2410-adc"
#define ADC_MINOR    1

int adc_phyread(int ain)
{
	int ret = 0;
  	//	adc_wait = wait;
	if (down_interruptible(&adc_lock))
		return -ERESTARTSYS;
	START_ADC_AIN(ain);       // ʹ��ADת��
	while((ADCCON&0x1000) == 0); //ѭ���ȴ�adת�����
	ret = ADCDAT0 ;
	up(&adc_lock);
//	DPRINTK("AIN[%d] = %d\n", ain, ret);
	return (ret & 0x3ff);
}

static ssize_t s3c2410_adc_read(struct file *file, char *buffer, size_t count, loff_t *ppos)
{
    unsigned int  ADCData;

 //   DPRINTK("s3c2410_adc_read,");

    ADCData = adc_phyread(Channel);  // �����ȡADת��ֵ
    copy_to_user(buffer, &ADCData, sizeof(ADCData)); // ����ȡ��ADת��ֵ�������û��ռ�
  //  DPRINTK("write:ADCData=0x%x,count=%d\n", ADCData, count);
    return sizeof(ADCData);
}

int s3c2410_adc_ioctl (struct inode *inode, struct file *file, unsigned int cmd, 
                  unsigned long arg)
{
	if(arg>7 || arg < 0)
	{
//		printk("error:adc channel is 0~7\n");
		return  -ERESTARTSYS;
	}
	switch(cmd)
	{
		case ADC_SET_CHANNEL:
			Channel = arg;
			break;
		default:
			break;	
	}
	return 0;
}

static int s3c2410_adc_open(struct inode *inode,struct file *filp)
{
  MOD_INC_USE_COUNT;
//  DPRINTK("open\n");
  return 0;
} 

static int s3c2410_adc_release(struct inode *inode,struct file *filp)
{
  MOD_DEC_USE_COUNT;
//  DPRINTK("release\n");
  return 0;
} 


static struct file_operations s3c2410_fops ={	// �豸�������ļ������ṹ��
    owner:   THIS_MODULE,
    open:    s3c2410_adc_open,
    read:    s3c2410_adc_read,
    ioctl:   s3c2410_adc_ioctl,
    write:   NULL,
    release: s3c2410_adc_release,
};

int __init s3c2410_adc_init(void)
{
	init_MUTEX(&adc_lock);
	ADCTSC = 0;	             /* ADC ��������ģʽ */
	DPRINTK("s3c2410_adc_init start\n");
		
	Major = register_chrdev(0,DEVICE_NAME,&s3c2410_fops);     // ע���ַ��豸
 	if(Major<0)
 	{
       	 	DPRINTK("can't get major number\n");
       		return Major;
  	}
#ifdef CONFIG_DEVFS_FS
    DPRINTK(" Major = %d\n", Major);
    devfs_adc_dir = devfs_mk_dir(NULL, "adc", NULL);    // �����豸�ļ�Ŀ¼
    devfs_adcraw = devfs_register(devfs_adc_dir,"0",DEVFS_FL_DEFAULT,
                    Major,ADC_MINOR,S_IFCHR|S_IRUSR|S_IWUSR,&s3c2410_fops,NULL);
#endif
  	DPRINTK("initialized\n");
  	return 0;
}

void __exit s3c2410_adc_exit(void)
{
#ifdef CONFIG_DEVFS_FS
       	devfs_unregister(devfs_adcraw);  // ж���豸�ļ�
  	devfs_unregister(devfs_adc_dir);   // ж���豸Ŀ¼
#endif
   	unregister_chrdev(Major,DEVICE_NAME);  // ע��AD�ַ��豸
}

module_init(s3c2410_adc_init);
module_exit(s3c2410_adc_exit);
MODULE_LICENSE("GPL");

