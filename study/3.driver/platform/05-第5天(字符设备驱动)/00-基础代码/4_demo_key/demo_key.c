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

static int demo_open(struct inode *pnode, struct file *filp)
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
	unsigned long len = min(count, sizeof(key));	//min�Ǹ��꣬������ȡ�������н�С��ֵ
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
	.open = demo_open,
	.release = demo_release,
};	


/*���ð����Ĵ�����ʽ��ͨ��������������*/
static void key_ioport_setting(void __iomem *gph_base, int trigger)
{
	unsigned long reg_val;

	/*@����gph3 ��0��1�˿�Ϊ����������trigger��Ӧ�ߵ͵�ƽ
	  *
	  *  ����con�Ĵ�����4λ��Ӧһ��io�˿�: B0000Ϊ���롢
	  *  B0001Ϊ�����B0010~B1110Ϊ������B1111Ϊ�ⲿ�ж�
	  *
	  *  ����dat�Ĵ�����1λ��Ӧһ��io�˿�:  B1 ��ʾ����/
	  *  ����ߵ�ƽ��B0��ʾ����/����͵�ƽ
	  */
	reg_val = readl(gph_base+GPH3_OFFSET+CONREG_OFFSET);
	reg_val &= ~((0x0f<<0) | (0x0f<<4));
	reg_val |= ((0x01<<0) | (0x01<<4) );	
	writel(reg_val, gph_base+GPH3_OFFSET+CONREG_OFFSET);

	reg_val = readl(gph_base+GPH3_OFFSET+DATREG_OFFSET);
	if(trigger == 0)	//����ǵ͵�ƽ��ʾ����������������͵�ƽ
		reg_val &= ~((0x01<<0) | (0x01<<1));
	else		//����������ߵ�ƽ
		reg_val |= (0x01<<0) | (0x01<<1);
	writel(reg_val, gph_base+GPH3_OFFSET+DATREG_OFFSET);


	/*@����gph2��3 4 5 6 7�˿�Ϊ���룬������������ʹ��(����trigger)��ּ��
	  *���û������ʱ����ڻ�ȡ���ĵ�ƽΪ�ߵ�ƽ
	  *
	  *  ����con�Ĵ�����4λ��Ӧһ��io�˿�: B0000Ϊ���롢
	  *  B0001Ϊ�����B0010~B1110Ϊ������B1111Ϊ�ⲿ�ж�
	  *
	  *  ����up�Ĵ�����2λ��Ӧһ��io�˿�:  B00 ��ʾ��ֹ
	  *  ��������B01��ʾ����ʹ�ܡ�B10��ʾ����ʹ�ܡ�B11����
	  */
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


static struct class *demo_class;
static int __init drvdemo_init(void)
{
	struct device *demo_device;
	int i;
	int retval;
	
	dem_dbg("[msg]:this is a driver demo, in module initial function\n");

	/*@ ����I/O�ڴ�(���������ַ�ռ�)
	  *    request_mem_region������linux��IO�ڴ�Ĺ�����˼ָ
	  *    ����ڴ����Ѿ�ռ���ˣ����˾Ͳ�Ҫ����
	  *@S5PV210_PA_GPH:  gph(0-3)����io�ڵ��������ַ
	  *@0x80:  ӳ�䷶Χ����λ�ֽ�
	  */
	gph_area = request_mem_region(S5PV210_PA_GPH, 0x80, "gph_area");
	if(gph_area == NULL){
		dem_dbg("[err]:gph can not request io!\n");
		retval = -ENXIO;		//�������ʾû������豸���ַ
		goto gph_region_err;
	}

	/*@ ӳ��gph(0-3) 4��I/O�������ַ���ڴ��ַ�ռ�(�����ַ)
	  *    ӳ��ɹ�������I/O�˿ڳ�Ϊ�ڴ��һ���֡���ʱ��
	  *    CPU���������һ���ڴ浥Ԫ������������I/O�˿ڣ�
	  *    ������Ҫ����ר�ŵ�����I/Oָ��
	  */
	gph_base = ioremap(S5PV210_PA_GPH, 0x80);
	if(gph_base == NULL){
		dem_dbg("[err]:gph can not map io!\n");
		retval = -ENXIO;		//�������ʾû������豸���ַ
		goto gph_map_err;
	}

	//������ӦI/O�ڳ�ʼ������
	key_ioport_setting(gph_base, 0);


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

	//ȡ��I/O�˿ڵ��ڴ�ӳ��
	iounmap(gph_base);
	//�黹�����I/O�ڴ�����
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
