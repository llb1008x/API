/*
 * Copyright (c) 2010 Samsung Electronics Co., Ltd.
 *              http://www.samsung.com/
 *
 * S5PC110 - LCD Driver for U-Boot
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */


#include "types.h"

#define CFG_LCD_FBUFFER				(0x48000000)

#define Inp32(_addr)		readl(_addr)
#define Outp32(addr, data)	(*(volatile u32 *)(addr) = (data))

#define LCD_WIDTH		800
#define LCD_HEIGHT		480

void LCD_Initialize(void)
{
	u32 uFbAddr = CFG_LCD_FBUFFER;

	u32 i;
	u32* pBuffer = (u32*)uFbAddr;

	/*GPD0_3����ߵ�ƽֱ�ӵ�������*/
	Outp32(0xe02000a0, (0x01<<3*4));
	Outp32(0xe02000a4, (0x01<<3));

	/*����LCD���ݼ����ƹ�������*/
	Outp32(0xe0200120, 0x22222222);	//set GPF0 as LVD_HSYNC,VSYNC,VCLK,VDEN,VD[3:0]
	Outp32(0xe0200128,0x0);			//set pull-up,down disable
	Outp32(0xe0200140, 0x22222222);	//set GPF1 as VD[11:4]
	Outp32(0xe0200148,0x0);			//set pull-up,down disable
	Outp32(0xe0200160, 0x22222222);	//set GPF2 as VD[19:12]
	Outp32(0xe0200168,0x0);			//set pull-up,down disable
	Outp32(0xe0200180, 0x00002222);	//set GPF3 as VD[23:20]
	Outp32(0xe0200188,0x0);			//set pull-up,down disable
	/* S5Pv210 EVT0 needs MAX drive strength*/
	Outp32(0xe020012c,0xffffffff);			
	Outp32(0xe020014c,0xffffffff);			
	Outp32(0xe020016c,0xffffffff);			
	Outp32(0xe020018c,0x3ff);				

	/*lcd��ؼĴ�������*/
/* VCLK = HCLK/4,��Ҫ��Ƶ,����video������ */
	Outp32(0xf8000000, (0x03<<6)|(1<<4)|3);
/* ����ʱ�ӷ�һ��,ʱ�������ش��� */
	Outp32(0xf8000004, (1<<5)|(1<<6)|(1<<7));
	Outp32(0xf8000010, (23<<16)|(22<<8)|(3));/*channel 0 Vertical Time, VBPD/VFPD/VSPW*/
	Outp32(0xf8000014, (46<<16)|(210<<8)|(3));/*channel 0 Horizontal Time, HBPD/HFPD/HSPW*/
	Outp32(0xf8000018, (((LCD_HEIGHT-1)<<11)|(LCD_WIDTH-1)));

/* Window0: DMA���ݽ�����word����,24bit RGB,����video������ */
	Outp32(0xf8000020, (1<<15)|(0xb<<2)|(1));/*Window0 ControlRegister*/
/* ʹ��Window0ͨ�� */
	Outp32(0xf8000034, 0x1);/*Windows channel enables 0*/

/* Window0����λ�� */
	Outp32(0xf8000044, (((LCD_WIDTH-1)<<11)|(LCD_HEIGHT-1)));
/* Window0���ڴ�С */
	Outp32(0xf8000048, (LCD_HEIGHT*LCD_WIDTH*4));

/* Window0���ڵ�֡������ʼ��ַ */
	Outp32(0xf80000a0, uFbAddr);
/* Window0���ڵ�֡���������ַ */
	Outp32(0xf80000d0, uFbAddr +(LCD_HEIGHT*LCD_WIDTH*4));
/* ʱ��·��ѡ�� */
	Outp32(0xe0107008,0x2); //syscon output path
}


