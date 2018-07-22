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


#include <linux/init.h>		//ָ��ģ�����ʼ�������
#include <linux/module.h>
#include <linux/kernel.h>

/*ģ���ʼ���������ú������ص��ں�ʱ�����á�
  * "__init" ��ʾ�ú������ڳ�ʼ��ʱʹ�ã�ģ��װ��
  * ��ϸú����ᱻ�ӵ�(�ͷ������ڴ�ռ�)�����
  * ��Ҫ�ڳ�ʼ������Ҫʹ�õĺ�������Ӵ˱��
  */
static int __init drvdemo_init(void)
{
	//printk������ʹ�ã���ο�<printk�Ĵ�ӡ����.txt>�ĵ�
	printk(KERN_WARNING "[msg]:this is a driver demo, in module initial function\n");
	return 0;
}

/*ģ�����������ģ����ں�ж��ʱ�����á�
  * "__exit" ��ʾ�ú���ֻ�ܱ�ж�ػ�ϵͳ�ر�ʱ���ã�
  * ��ģ���ֱ�ӱ��뵽�ں˻��ں�����Ϊ������ж
  * ��ģ��ʱ�ú����ᱻ�򵥵Ķ���
  */
static void __exit drvdemo_exit(void)
{
	printk(KERN_WARNING "[msg]:in module exit function\n");
}

module_init(drvdemo_init);
module_exit(drvdemo_exit);

/*Ϊ����ģ��ָ�����֤�������г����õ����֤��
 *ѡ��һ���ɣ����û����ʾָ������Ĭ��Ϊר�е�
 */
MODULE_LICENSE("GPL"); 	//GNUͨ�ù������֤
//MODULE_LICENSE("GPL_v2");		//���֤(GPL�汾2)
//MODULE_LICENSE("Dual BSD/GPL");	//BSD/GPL˫�����֤
//MODULE_LICENSE("Proprietary");	//ģ���Ϊר�е�

MODULE_AUTHOR("sunplusedu");		//ģ������(��ѡ)
MODULE_DESCRIPTION("used for study linux drivers");	//ģ������(��ѡ)
