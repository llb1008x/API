/*
 * Copyright  2011-2012  sunplusapp
 * driver demo for sunplusedu
 */

/*
 * this program is free software; you can redistribute it and/or modify
 * it
 * 
 * Date and Edition:		2012-12-27  v1.3
 * Date and Edition:		2013-12-02  v2.0
 * Author:				"driving team" of sunplusedu
 */

#include <linux/init.h>	
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/device.h>
/*����readl��readl����*/
#include <asm/io.h>

#include "key_ioctl.h"

#define	DEMO_DEBUG
#ifdef 	DEMO_DEBUG
#define	dem_dbg(fmt, arg...)		printk(KERN_WARNING fmt, ##arg)
#else
#define	dem_dbg(fmt, arg...)		printk(KERN_DEBUG fmt, ##arg)
#endif

#define	DEVICE_COUNT			1

/*io��Ӳ����Ϣ�ĺ궨�壬�Ĵ������������ַ
  *�����Ĵ�����ƫ����
  */
#define	S5PV210_PA_GPH		0xE0200C00		

#define	GPH2_OFFSET			0X40
#define	GPH3_OFFSET			0X60
#define	CONREG_OFFSET			0x00
#define	DATREG_OFFSET			0x04
#define	UPREG_OFFSET			0x08
#define	DRV_OFFSET				0x0c

//ӳ��õ������׵�ַ��io��Դ
static void __iomem *gph_base = NULL;
static struct resource *gph_area = NULL;

static int major = 0;

static int demo_open (struct inode *pnode, struct file *filp)
{
	dem_dbg("[kern func]: %s  major: %d  minor: %d\n",
					__FUNCTION__, imajor(pnode), iminor(pnode));
	return 0;
}

/*@��ֵ���뺯��
  *   �������Ϊghp2 dat�Ĵ�����ֵ�������߼��ļ�ֵ
  */
static int key_convert(unsigned long reg_val)
{
	int key;
	switch(reg_val){
		case 0x08:	//0000 1xxx
		case 0xf0:	//1111 0xxx			
			key=2; break;
		case 0x10:	//00010xxx
		case 0xe8:	//1110 1xxx			
			key=3; break;
		case 0x20:	//0010 0xxx
		case 0xd8:	//1101 1xxx			
			key=9; break;
		case 0x40:	//0100 0xxx
		case 0xb8:	//1011 1xxx
			key=10; break;
		case 0x80:	//1000 0xxx
		case 0x78:	//0111 1xxx
			key=11; break;
		default:
			key=-1; break;
	}
	return key;
}

static ssize_t demo_read (struct file *filp, char __user *buf, size_t count, loff_t *offp)
{
	int key = -1;
	unsigned long reg_val;
	unsigned long len = min(count, sizeof(key));	
	int retval;
	
	/*��ȡdat�Ĵ�����ֵ����Ӧ�˰������*/
	reg_val = readl(gph_base+GPH2_OFFSET+DATREG_OFFSET);
	//��dat�Ĵ����е�ֵת��Ϊ�߼���ֵ
	key = key_convert(reg_val & 0xf8);

	if(copy_to_user(buf, &key, len) != 0){
		retval = -EFAULT;
		goto cp_err;
	}

	return len;
cp_err:
	return retval;	
}

/*���ð����Ĵ�����ʽ��ͨ��������������*/
static void key_ioport_setting(void __iomem *gph_base, int trigger)
{
	unsigned long reg_val;

	reg_val = readl(gph_base+GPH3_OFFSET+CONREG_OFFSET);
	reg_val &= ~((0x0f<<0) | (0x0f<<4));
	reg_val |= ((0x01<<0) | (0x01<<4) );	
	writel(reg_val, gph_base+GPH3_OFFSET+CONREG_OFFSET);

	reg_val = readl(gph_base+GPH3_OFFSET+DATREG_OFFSET);
	if(trigger == 0)
		reg_val &= ~((0x01<<0) | (0x01<<1));
	else
		reg_val |= (0x01<<0) | (0x01<<1);
	writel(reg_val, gph_base+GPH3_OFFSET+DATREG_OFFSET);

	reg_val = readl(gph_base+GPH2_OFFSET+CONREG_OFFSET);
	reg_val &= ~((0x0f<<12) | (0x0f<<16) | (0x0f<<20) | (0x0f<<24) |(0x0f<<28));
	writel(reg_val, gph_base+GPH2_OFFSET+CONREG_OFFSET);

	reg_val = readl(gph_base+GPH2_OFFSET+UPREG_OFFSET);
	reg_val &= ~((0x03<<6) | (0x03<<8) | (0x03<<10) | (0x03<<12) |(0x03<<14));
	if(trigger == 0)
		reg_val |= ((0x02<<6) | (0x02<<8) | (0x02<<10) | (0x02<<12) |(0x02<<14));	
	else
		reg_val |= ((0x01<<6) | (0x01<<8) | (0x01<<10) | (0x01<<12) |(0x01<<14));
	writel(reg_val, gph_base+GPH2_OFFSET+UPREG_OFFSET);
	
}

/*@ ����ioctl�ӿڣ�ʹ�û�����ѡ�����ð����Ĵ�����ʽ��
  *    ioctl����һ��������ԣ����в���cmd��arg�������ж���������
  *    ������ʹ�ò���cmd���ð����Ĵ�����ʽ��arg����δʹ��
  */
static int demo_ioctl(struct inode *pnode, struct file *filp, unsigned int cmd, unsigned long arg)
{
	dem_dbg("[kern func]: %s  major: %d  minor: %d\n",
					__FUNCTION__, imajor(pnode), iminor(pnode));
	
	printk("[msg]:the arg value: %d\n",arg);
	switch(cmd){
		case KEY_TRIGGER_LOW:
			key_ioport_setting(gph_base, 0);
			break;
		case KEY_TRIGGER_HIGH:
			key_ioport_setting(gph_base, 1);
			break;
		default:
			break;
	}
	return 0;
}

static int demo_release (struct inode *pnode, struct file *filp)
{
	dem_dbg("[kern func]: %s  major: %d  minor: %d\n",
					__FUNCTION__, imajor(pnode), iminor(pnode));
	return 0;
}


/*@����file_operations�ṹ�����*/
static struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = demo_read,
	.ioctl = demo_ioctl,
	.open = demo_open,
	.release = demo_release,
};	

/*  @����IO�ڵĳ�ʼ����������ioctl�ӿ���ɣ���Ҫ�û�����ioctl����
static void key_ioport_init(void __iomem *gph_base)
{
	unsigned long reg_val;

	reg_val = readl(gph_base+GPH3_OFFSET+CONREG_OFFSET);
	reg_val &= ~((0x0f<<0) | (0x0f<<4));
	reg_val |= ((0x01<<0) | (0x01<<4) );	
	writel(reg_val, gph_base+GPH3_OFFSET+CONREG_OFFSET);

	reg_val = readl(gph_base+GPH3_OFFSET+DATREG_OFFSET);
	reg_val &= ~((0x01<<0) | (0x01<<1));
	writel(reg_val, gph_base+GPH3_OFFSET+DATREG_OFFSET);

	reg_val = readl(gph_base+GPH2_OFFSET+CONREG_OFFSET);
	reg_val &= ~((0x0f<<12) | (0x0f<<16) | (0x0f<<20) | (0x0f<<24) |(0x0f<<28));
	//reg_val |= ((0x00<<12) | (0x00<<16) | (0x00<<20) | (0x00<<24) |(0x00<<28));	
	writel(reg_val, gph_base+GPH2_OFFSET+CONREG_OFFSET);

	reg_val = readl(gph_base+GPH2_OFFSET+UPREG_OFFSET);
	reg_val &= ~((0x03<<6) | (0x03<<8) | (0x03<<10) | (0x03<<12) |(0x03<<14));
	reg_val |= ((0x02<<6) | (0x02<<8) | (0x02<<10) | (0x02<<12) |(0x02<<14));	
	writel(reg_val, gph_base+GPH2_OFFSET+UPREG_OFFSET);
	
}
*/
static struct class *demo_class;
static int __init drvdemo_init(void)
{
	struct device *demo_device;
	int i;
	int retval;
	
	dem_dbg("[msg]:this is a driver demo, in module initial function\n");

	gph_area = request_mem_region(S5PV210_PA_GPH, 0x80, "gph_area");
	if(gph_area == NULL){
		dem_dbg("[err]:gph can not request io!\n");
		retval = -ENXIO;		//�������ʾû������豸���ַ
		goto gph_region_err;
	}

	gph_base = ioremap(S5PV210_PA_GPH, 0x80);
	if(gph_base == NULL){
		dem_dbg("[err]:gph can not map io!\n");
		retval = -ENXIO;		//�������ʾû������豸���ַ
		goto gph_map_err;
	}

	//������ӦI/O�ڳ�ʼ������
	//key_ioport_init(gph_base);

	major = register_chrdev(0, "demo_chrdev", &fops);
	if(major < 0){
		retval = major;
		goto chrdev_err;
	}

	demo_class = class_create(THIS_MODULE,"demo_class");
	if(IS_ERR(demo_class)){
		retval =  PTR_ERR(demo_class);
		goto class_err;
	}

	for(i=0; i<DEVICE_COUNT; i++){	//���ɴ���255���豸�ڵ�(register_chrdev����������0-254��Χ�Ĵ��豸��)
		demo_device = device_create(demo_class,NULL, MKDEV(major, i), NULL,"demo%d",i);
		if(IS_ERR(demo_device)){
			retval = PTR_ERR(demo_device);
			goto device_err;
		}
	}
	return 0;
	
device_err:
	while(i--)	//�豸�ڵ㴴���Ļع�����
		device_destroy(demo_class,MKDEV(major, i));
	class_destroy(demo_class);
	
class_err:
	unregister_chrdev(major, "demo_chrdev");
	
chrdev_err:
	iounmap(gph_base);
	
gph_map_err:
	release_mem_region(S5PV210_PA_GPH, 0x80);
	
gph_region_err:
	return retval;
}

static void __exit drvdemo_exit(void)
{
	int i;
	
	dem_dbg("[msg]:in module exit function\n");

	iounmap(gph_base);
	release_mem_region(S5PV210_PA_GPH, 0x80);
	
	unregister_chrdev(major, "demo_chrdev");

	for(i=0; i<DEVICE_COUNT; i++)
		device_destroy(demo_class,MKDEV(major, i));
	class_destroy(demo_class);
}

module_init(drvdemo_init);
module_exit(drvdemo_exit);


MODULE_LICENSE("Dual BSD/GPL");	//BSD/GPL˫�����֤
MODULE_AUTHOR("sunplusedu");		//ģ������(��ѡ)
MODULE_DESCRIPTION("used for studing linux drivers");	//ģ������(��ѡ)
