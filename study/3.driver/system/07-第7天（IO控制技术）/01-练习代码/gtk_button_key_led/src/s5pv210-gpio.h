#ifndef __GPIODRIVER_H__
#define __GPIODRIVER_H__

//io������
//gpA0 gpA1 gpB gpC0 gpC1 gpD0 gpD1 gpE0 
//gpF0 gpF1 gpF2 gpF3
//gpG0 gpG1 gpG2 gpG3 gpH0 gpH1 gpH2 gpH3 
//gpI gpJ0 gpJ1 gpJ2 gpJ3 gpJ4

#define GPIO_SET_PIN_OUT			0x01	//����ָ���ܽ�Ϊ���
#define GPIO_SET_PIN_IN				0x00	//����ָ���ܽ�Ϊ����

#define GPIO_CLR_PIN				0x10	//����ָ���ܽ�Ϊ�͵�ƽ
#define GPIO_SET_PIN				0x11	//����ָ���ܽ�Ϊ�ߵ�ƽ

//type: 0 - disabled, 1 - pull down, 2 - pull up
#define GPIO_SET_PULL_DIS			0x20	//��ָֹ���ܽ�����������
#define GPIO_SET_PULL_DOWN			0x21	//����ָ���ܽ�����ʹ��
#define GPIO_SET_PULL_UP		 	0x22	//����ָ���ܽ�����ʹ��

#define GPIO_SET_PIN_DRV0			0x30	//����io����������Ϊ1��
#define GPIO_SET_PIN_DRV1			0x31	//����io����������Ϊ2��
#define GPIO_SET_PIN_DRV2			0x32	//����io����������Ϊ3��

#define BIT(x) (0x01)<<(x)

#endif//__GPIODRIVER_H__
