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
/*�ṹ��file_operations�����ͷ�ļ�*/
#include <linux/fs.h>
/*����copy_to/from_user������ͷ�ļ�*/
#include <linux/uaccess.h>
/*����class_create ��device_create�����Ϣ*/
#include <linux/device.h>

#define	DEMO_DEBUG
#ifdef 	DEMO_DEBUG
#define	dem_dbg(fmt, arg...)		printk(KERN_WARNING fmt, ##arg)
#else
#define	dem_dbg(fmt, arg...)		printk(KERN_DEBUG fmt, ##arg)
#endif

#define	DEVICE_COUNT			2


/*��¼��ǰ������ռ�õ����豸��*/
static int major = 0;

static int demo_open (struct inode *pnode, struct file *filp)
{
	dem_dbg("[kern func]: %s  major: %d  minor: %d\n",
					__FUNCTION__, imajor(pnode), iminor(pnode));
	return 0;
}

static ssize_t demo_read (struct file *filp, char __user *buf, size_t count, loff_t *offp)
{
	unsigned char ary[100] = "you are reading successfully!";
	unsigned long len = min(count, sizeof(ary));	//min�Ǹ��꣬������ȡ�������н�С��ֵ
	int retval;

	dem_dbg("[kern func]: %s  major: %d  minor: %d\n",
					__FUNCTION__, imajor(filp->f_dentry->d_inode), 
					iminor(filp->f_dentry->d_inode));
	
	//file�ṹ���f_flags��Ա�������ж��Ƿ�������ȡ��Ȼ�������Ӧ����

	if(copy_to_user(buf, ary, len) != 0){
		retval = -EFAULT;
		goto cp_err;
	}

	return len;	//�ɹ�����ʵ�ʴ�����ֽ���
cp_err:
	return retval;	
}

static ssize_t demo_write(struct file *filp, const char __user *buf, size_t count, loff_t *offp)
{
	unsigned char ary[100] = "";
	unsigned long len = min(count, sizeof(ary));	//min�Ǹ��꣬������ȡ�������н�С��ֵ
	int retval;

	dem_dbg("[kern func]: %s  major: %d  minor: %d\n",
					__FUNCTION__, imajor(filp->f_dentry->d_inode), 
					iminor(filp->f_dentry->d_inode));
	
	if(copy_from_user(ary, buf, len) != 0){
		retval = -EFAULT;
		goto cp_err;
	}

	printk("[msg]: writing context: %s\n",ary);

	return len;	//�ɹ�����ʵ�ʴ�����ֽ���
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
	.write = demo_write,
	.open = demo_open,
	.release = demo_release,
};	

static struct class *demo_class;
static int __init drvdemo_init(void)
{
	struct device *demo_device;
	int i;
	int retval;
	
	dem_dbg("[msg]:this is a driver demo, in module initial function\n");

	/*ע���ַ������������ɹ����ض�̬����õ����豸�ţ�ʧ��
	 *���ش�����(��ֵ)*/
	major = register_chrdev(0, "demo_chrdev", &fops);
	if(major < 0){
		retval = major;
		goto chrdev_err;
	}

	/*�����豸�ࣨ�豸ģ�����֪ʶ�����ڿγ��н��ܣ�*/
	demo_class = class_create(THIS_MODULE,"demo_class");
	if(IS_ERR(demo_class)){
		retval =  PTR_ERR(demo_class);
		goto class_err;
	}

	/*�����豸�ļ���֪ͨ�û��ڡ�/dev/��Ŀ¼�´�������ΪdemoX���豸�ļ�*/
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
	class_destroy(demo_class);	//ɾ���豸��
class_err:
	unregister_chrdev(major, "demo_chrdev");
chrdev_err:
	return retval;
}

static void __exit drvdemo_exit(void)
{
	int i;
	
	dem_dbg("[msg]:in module exit function\n");
	/*ע���ַ������������޷���ֵ��majorΪ�ѷ�������豸��*/
	unregister_chrdev(major, "demo_chrdev");
	/*ɾ���豸�ڵ���豸��*/
	for(i=0; i<DEVICE_COUNT; i++)
		device_destroy(demo_class,MKDEV(major, i));
	class_destroy(demo_class);
}

module_init(drvdemo_init);
module_exit(drvdemo_exit);


MODULE_LICENSE("Dual BSD/GPL");	//BSD/GPL˫�����֤
MODULE_AUTHOR("sunplusedu");		//ģ������(��ѡ)
MODULE_DESCRIPTION("used for studing linux drivers");	//ģ������(��ѡ)
