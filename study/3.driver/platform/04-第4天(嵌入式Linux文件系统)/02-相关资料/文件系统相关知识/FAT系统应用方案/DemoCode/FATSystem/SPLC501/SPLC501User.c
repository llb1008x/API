//***************************************************************************//
//�ļ�������SPLC501�û��ӿ���������
//
//������	С��	20050519	Sunnorth	Mz Design
//
//
//***************************************************************************//
#include	"SPLC501User.h"
extern unsigned int Asii8;
extern unsigned int Asii16;

extern void F_SPLC501_Initial_IO(void);
extern void F_SPLC501_Write_Com(unsigned int Command);
extern void F_SPLC501_Write_Data(unsigned int SendData);
extern unsigned int F_SPLC501_Read_Data(void);
extern unsigned int F_SPLC501_Read_Status(void);

unsigned int CurPaintMode;				//Show Mode Cover of XOR
unsigned int CurPowerMode;				//Show the Power Mode of LCD501
unsigned int CurLCDSet;					//Show the LCD Set�Ҷ�
unsigned int CurScroll;					//Show the scroll dotes
unsigned int CurScrMode;				//��ʾ����/���ࣨ1/0��
unsigned int CurASCIIFont;				//��ǰASCII����ֺţ�8X16/6X8��

//***************************************************************************//
//����F_LCD_ON_OFF(unsigned int SetCom)
//������Һ�����غ���  C���Ե��� 
//������unsigned int SetCom		SetCom=0:LCD Off  SetCom!=0:LCD On
//������С��	20050519	sunnorth  Mz Design
//ע�⣺�ƻ�R1�Ĵ���������R1
//***************************************************************************//
void F_LCD_ONOFF(unsigned int SetCom)
{
	if(SetCom)							//ON LCD
		F_SPLC501_Write_Com(M_LCD_ON);
	else								//OFF LCD
		F_SPLC501_Write_Com(M_LCD_OFF);
}
//***************************************************************************//
//����LCD501_Init(unsigned int InitialData)
//������Һ����ʾ��ʼ������  C���Ե��� 
//������unsigned int InitialData	��ʼ����ʾ����
//������xinqiang	20050519	sunnorth  Mz Design
//ע�⣺�ƻ�R1�Ĵ���������R1
//***************************************************************************//
void LCD501_Init(unsigned int InitialData)
{
	unsigned int i,j;
	F_SPLC501_Initial_IO();	//Initial LCD IO
	F_SPLC501_Write_Com(M_LCD_ON);							//LCD On
	F_SPLC501_Write_Com(M_LCD_POWER_ALL);		//�����ϵ����ģʽ
	
	F_SPLC501_Write_Com(M_LCD_ELE_VOL);			//��������ģʽ����ʾ���ȣ�
	F_SPLC501_Write_Com(0x0012);				//ָ������0x0000~0x003f
	
	F_SPLC501_Write_Com(M_LCD_VDD_SET);			//V5�ڲ���ѹ���ڵ�������
	F_SPLC501_Write_Com(M_LCD_VDD);			//LCDƫѹ���ã�V3ʱѡ
	
	F_SPLC501_Write_Com(M_LCD_COM_NOR);			//Com ɨ�跽ʽ����
	F_SPLC501_Write_Com(M_LCD_SEG_REV);			//Segment����ѡ��
	F_SPLC501_Write_Com(M_LCD_ALL_LOW);			//ȫ������/�䰵ָ��
	F_SPLC501_Write_Com(M_LCD_ALL_NOR);			//��������ʾ����ָ��
	
	F_SPLC501_Write_Com(M_LCD_STATIC_OFF);		//�رվ�ָ̬ʾ��
	F_SPLC501_Write_Com(0x0000);				//ָ������
	
//	F_SPLC501_Write_Com(0x00ee);				//��д��ģʽ����ָ��
	F_SPLC501_Write_Com(M_LCD_BEGIN_LINE+0);	//������ʾ��ʼ�ж�ӦRAM
	
	for(i=0;i<8;i++)							//��LCD������ʼ������ʾ����
	{
		F_SPLC501_Write_Com(M_LCD_COL_PAGE+i);
		F_SPLC501_Write_Com(M_LCD_COL_LINE_LOW);
		F_SPLC501_Write_Com(M_LCD_COL_LINE_HIG);
		for(j=0;j<128;j++)
		{
			F_SPLC501_Write_Data(InitialData);
		}
	}
	CurPaintMode = 0;
	CurPowerMode = 1;			//now is normal mode
	CurLCDSet = 8;
	CurScroll = 0;
	CurScrMode = 1;
	CurASCIIFont = 1;
}
//======================================================
// ����	void LCD501_Power(unsigned int ModeCode)
// ������	SPLC501��Դ����,�������ָ����ģʽ������LCD��SLEEP��STDBYģʽ��ʱ��
//			���øú�����ʹLCD����
// ������		ModeCode - ���������в���֮һ��
//				1	PWR_ON: 	��������ģʽ
//				2	PWR_SLEEP:	˯��ģʽ����Ļ����ʾ���������ڻ��Ѻ��Ա���
//				3	PWR_STDBY: ����ģʽ����SLEEPʡ�磬����ʾ���ݻᶪʧ
//				0	PWR_OFF:	�رյ�Դ
// ������	xinqiang	20050624	sunnorth  Mz Design
// ע�⣺		��
//======================================================
void LCD501_Power(unsigned int ModeCode)
{
	if(CurPowerMode == PWR_SLEEP)
	{
		F_SPLC501_Write_Com(M_LCD_ALL_LOW);
		F_SPLC501_Write_Com(M_LCD_STATIC_ON);
		F_SPLC501_Write_Com(0x0003);
		CurPowerMode = 1;
	}
	else if(CurPowerMode == PWR_STDBY)
	{
		F_SPLC501_Write_Com(M_LCD_ON);
		F_SPLC501_Write_Com(M_LCD_ALL_LOW);
		CurPowerMode = 1;
	}
	else{
		switch(ModeCode)
		{
			case PWR_ON:
				F_SPLC501_Write_Com(M_LCD_ON);
				break;
			case PWR_SLEEP:
				F_SPLC501_Write_Com(M_LCD_STATIC_OFF);
				F_SPLC501_Write_Com(0x0000);
				F_SPLC501_Write_Com(M_LCD_OFF);
				F_SPLC501_Write_Com(M_LCD_ALL_LIGNT);
				break;
			case PWR_STDBY:
				F_SPLC501_Write_Com(M_LCD_STATIC_ON);
				F_SPLC501_Write_Com(0x0003);
				F_SPLC501_Write_Com(M_LCD_OFF);
				F_SPLC501_Write_Com(M_LCD_ALL_LIGNT);
				break;
			default://POW_OFF
				F_SPLC501_Write_Com(M_LCD_OFF);
				break;
		}//
		CurPowerMode = ModeCode;
	}
}
//***************************************************************************//
// ����	void LCD501_SetContrast(unsigned int Contrast)
// ������	������ʾ�Աȶ�   C���Ե���
// ������	Contrast - �Աȶ�ֵ����Χ��0��31����ֵԽ��Աȶ�Խ��
// ���أ�	��
// ������	xinqiang	20050624	Mz Design
// ע�⣺   ��
//***************************************************************************//
void LCD501_SetContrast(unsigned int Contrast)
{
	if(Contrast>0x003f)
		Contrast = 0x003f;
	F_SPLC501_Write_Com(M_LCD_ELE_VOL);			//��������ģʽ����ʾ���ȣ�
	F_SPLC501_Write_Com(Contrast);			//��������ģʽ����ʾ���ȣ�
	CurLCDSet = Contrast;
}
//***************************************************************************//
// ����	unsigned int LCD501_GetContrast(void)
// ������	��ȡ��ʾ�Աȶ�   C���Ե���
// ������	��
// ���أ�	��ǰ��ʾ�ĶԱȶ�ֵ
// ������	xinqiang	20050624	Mz Design
// ע�⣺   ��
//***************************************************************************//
unsigned int LCD501_GetContrast(void)
{
	return CurLCDSet;//
}
//***************************************************************************//
// ����	void LCD501_ScrollUp(unsigned int Rows)
// ������	��Ļ���Ϲ���   C���Ե���
// ������	Rows  ��Ļ���Ϲ���������������Χ��1��64
// ���أ�	null
// ������	xinqiang	20050624	Mz Design
// ע�⣺   ��
//***************************************************************************//
void LCD501_ScrollUp(unsigned int Rows)
{
	CurScroll = CurScroll+Rows;
	while(CurScroll>=SCR_MAX_ROW)
		CurScroll -= SCR_MAX_ROW;
	F_SPLC501_Write_Com(M_LCD_BEGIN_LINE+CurScroll);	//������ʾ��ʼ�ж�ӦRAM
}
//***************************************************************************//
// ����	unsigned int LCD501_GetScroll(void)
// ������	��ȡ��Ļ�Ѿ����Ϲ�����ֵ   C���Ե���
// ������	��
// ���أ�	��Ļ�Ѿ����Ϲ�����ֵ
// ������	xinqiang	20050624	Mz Design
// ע�⣺   ��
//***************************************************************************//
unsigned int LCD501_GetScroll(void)
{
	return CurScroll;//
}
//***************************************************************************//
// ����	void LCD501_ReverseColor(void)
// ������	��Ļ��ɫ��ʾ��ÿִ��һ�θú�������Ļ��ɫ��תһ�Ρ�   C���Ե���
// ������	��
// ���أ�	��
// ������	xinqiang	20050624	Mz Design
// ע�⣺   ��
//***************************************************************************//
void LCD501_ReverseColor(void)
{
	if(CurScrMode)//
	{
		F_SPLC501_Write_Com(M_LCD_ALL_REV);			//��������ʾ����ָ��
		CurScrMode = 0;
	}
	else
	{
		F_SPLC501_Write_Com(M_LCD_ALL_NOR);			//��������ʾ����ָ��
		CurScrMode = 1;
	}
}
//***************************************************************************//
//����LCD501_ClrScreen(unsigned int Mode)
//������Һ��������������  C���Ե��� 
//������unsigned int Mode	��������Mode=1 ��ȫ������Mode=0 ��ȫ����
//������С��	20050519	sunnorth  Mz Design
//ע�⣺�ƻ�R1�Ĵ���������R1
//***************************************************************************//
void LCD501_ClrScreen(unsigned int Mode)
{
	unsigned int i,j;
	if(Mode)
		Mode = 0x00ff;
	else
		Mode = 0x0000;
		
	for(i=0;i<8;i++)
	{
		F_SPLC501_Write_Com(M_LCD_COL_PAGE+i);
		F_SPLC501_Write_Com(M_LCD_COL_LINE_LOW);
		F_SPLC501_Write_Com(M_LCD_COL_LINE_HIG);
		F_SPLC501_Write_Com(0xee);					//����дģʽ����ָ��
		for(j=0;j<128;j++)
		{
			F_SPLC501_Write_Data(Mode);
		}
	}
}
//***************************************************************************//
//����F_LCD_LineRun(unsigned int line)
//������Һ����ʾ��ʼ�����ú���  C���Ե���  �����������ã� 
//������unsigned int line	��ʼRAM�У�0~63
//������С��	20050527	sunnorth  Mz Design
//ע�⣺�ƻ�R1�Ĵ���������R1
//***************************************************************************//
void F_LCD_LineRun(unsigned int line)
{
	if(line>=64)
		line = 0;
	F_SPLC501_Write_Com(M_LCD_BEGIN_LINE+line);	//������ʾ��ʼ�ж�ӦRAM
}
//***************************************************************************//
//����void F_LCD_WOSCASII(unsigned int x,unsigned int page,unsigned int d)
//������OSDȡ��ASCII�ַ���ʾ����  C���Ե��� 
//������x ��ʾ�ַ�����ʼ�У�0~128��   page ��ʾ�ַ�����ʼҳ��0~6��
//		d ASCII��
//������С��	20050519	sunnorth  Mz Design
//ע�⣺��������ʾ��OSDȡ��ģ����ȡ��16X16��16X12��ģ��ASCII�ַ�
//***************************************************************************//
/*void F_LCD_WOSCASII(unsigned int x,unsigned int page,unsigned int d)
{
	unsigned int untype;
	unsigned int *word;
	int i,j,m;
	unsigned int x_low,x_hight;
	
	d = d-33;
	word = (unsigned int)encoding_00+(d*17);
	
	untype = *(word++);
	i = untype>>8;
	j = (untype&0xff);
	x_low = (x&0x000f)+ M_LCD_COL_LINE_LOW;				//��λ�е�ַ���õĵ�λָ��
	x_hight = ((x>>4)&0x000f)+M_LCD_COL_LINE_HIG;		//��λ�е�ַ���õĸ�λָ��
		
	for(;i>0;)
	{
		i = i-8;
		F_SPLC501_Write_Com(M_LCD_COL_PAGE+page);
		F_SPLC501_Write_Com(x_low);
		F_SPLC501_Write_Com(x_hight);
		for(m=0;m<j;m++)
		{
			if(i)
				untype = *(word+m);
			else
				untype = (*(word+m)>>8);
			untype = untype&0x00ff;
			F_SPLC501_Write_Data(untype);
		}
		page++;
	}
}*/
//***************************************************************************//
// ����	void LCD501_SetPaintMode(unsigned int ModeCode)
// ������	����ͼ����ʾģʽ	
// ������	ModeCode - ͼ����ʾģʽ��������������ֵ֮һ��
//			0	PAINT_COVER:	������ʾ��ͼ������λ�õ�ԭ����ʾ���ݽ������ǵ�
//			1	PAINT_SET:		������ʾ��ͼ������λ����ԭ�еĺ�ɫ�㽫������
//			2	PAINT_CLR:		������ʾ��ͼ�η�ɫ��ʾ����ͼ������λ����ԭ�еİ�
//								ɫ�㽫������
//			3	PAINT_REV:		�����ʾ��ͼ����ÿһ������ԭ�еĵ�����򣬼����
//								ͼ����ĳ�����ɫ��������λ��ԭ�е���ɫ��ͬ����
//								�õ���ʾ��ɫ���������ɫ��ԭ����ɫ��ͬ����õ�
//								��ʾ��ɫ
//			4	PAINT_RECO      ȡ����������ʾ��ԭʼͼ������ȡ����Ȼ��ͼ������λ��
//								��ԭ����ʾ���ݽ������ǵ�
// ���أ�		��
// ������	xinqiang	20050810	Mz Design
// ע�⣺
//***************************************************************************//
void LCD501_SetPaintMode(unsigned int ModeCode)
{
	if(ModeCode > 4)
		ModeCode = PAINT_COVER;
	CurPaintMode = ModeCode;
}
//***************************************************************************//
// ����	unsigned int LCD501_GetPaintMode(void)
// ������	����ͼ����ʾģʽ
// ������   ��	
// ���أ�	ModeCode - ͼ����ʾģʽ��������������ֵ֮һ��
//			0	PAINT_COVER:	������ʾ��ͼ������λ�õ�ԭ����ʾ���ݽ������ǵ�
//			1	PAINT_SET:		������ʾ��ͼ������λ����ԭ�еĺ�ɫ�㽫������
//			2	PAINT_CLR:		������ʾ��ͼ�η�ɫ��ʾ����ͼ������λ����ԭ�еİ�
//								ɫ�㽫������
//			3	PAINT_REV:		�����ʾ��ͼ����ÿһ������ԭ�еĵ�����򣬼����
//								ͼ����ĳ�����ɫ��������λ��ԭ�е���ɫ��ͬ����
//								�õ���ʾ��ɫ���������ɫ��ԭ����ɫ��ͬ����õ�
//								��ʾ��ɫ
//			4	PAINT_RECO      ȡ����������ʾ��ԭʼͼ������ȡ����Ȼ��ͼ������λ��
//								��ԭ����ʾ���ݽ������ǵ�
// ������	xinqiang	20050810	Mz Design
// ע�⣺
//***************************************************************************//
unsigned int LCD501_GetPaintMode(void)
{
	return CurPaintMode;
}
//***************************************************************************//
//����void LCD501_PutPixel(unsigned int x,unsigned int y)
//�����������ӳ���
//������x Ҫ�����x����   y Ҫ���ĵ��y����  
//���أ���
//������xinqiang	20050521	sunnorth  Mz Design
//ע�⣺��
//***************************************************************************//
void LCD501_PutPixel(unsigned int x,unsigned int y)
{
	unsigned int x_low,x_hight;			//�����е�ַ�ĸߵ�λָ��
	unsigned int Dot_Mask_Buf=0x0001;
	unsigned int y_Page;			//���ڴ��Ҫ�����λ�����ڵ�byte����λ��
	x = x+4;
	x_low = (x&0x000f);					//��λ�е�ַ���õĵ�λָ��
	x_hight = ((x>>4)&0x000f)+0x10;		//��λ�е�ַ���õĸ�λָ��
	y_Page = y&0x07;					//ȡ����λ
	while(y_Page!=0)
	{
		Dot_Mask_Buf = Dot_Mask_Buf<<1;
		y_Page--;
	}
	y_Page = y>>3;						//Get the page of the byte
	F_SPLC501_Write_Com(M_LCD_COL_PAGE+y_Page);
	F_SPLC501_Write_Com(x_low);
	F_SPLC501_Write_Com(x_hight);
	F_SPLC501_Write_Com(0xe0);					//����Ϊ��д��ģʽ
	y_Page = F_SPLC501_Read_Data();					//
	switch(CurPaintMode)
	{
		case PAINT_CLR: y_Page = y_Page&(~Dot_Mask_Buf);
			break;
	//	case PAINT_SET: y_Page = y_Page|Dot_Mask_Buf;
	//		break;
		case PAINT_REV: x = y_Page^Dot_Mask_Buf; x = x&Dot_Mask_Buf;
				y_Page = (y_Page&(~Dot_Mask_Buf))|x;
			break;
		default: y_Page = y_Page|Dot_Mask_Buf;
			break;
	}
	F_SPLC501_Write_Data(y_Page);					//
	F_SPLC501_Write_Com(0xee);					//��д��ģʽ����
}
//***************************************************************************//
//����void LCD501_Line(unsigned int x,unsigned int y,unsigned int e_x,
//							unsigned int e_y)
//��������ֱ�߳���
//������x Ҫ���ߵ�x�������   		 y Ҫ�����ߵ�y�������  
//		e_x Ҫ���ߵ�x �յ�����  	 e_y Ҫ�����ߵ�y�������
//���أ���
//������С��	200505121	sunnorth  Mz Design
//ע�⣺
//***************************************************************************//
void LCD501_Line(unsigned int x,unsigned int y,unsigned int e_x,unsigned int e_y)
{
	int Offset_x,Offset_y,Offset_k = 0;
	int Err_d = 1;
	if(y>e_y)
	{
		Offset_x = x;
		x = e_x;
		e_x = Offset_x;
		Offset_x = y;
		y = e_y;
		e_y = Offset_x;
	}
	Offset_x = e_x-x;
	Offset_y = e_y-y;
	LCD501_PutPixel(x,y);
	if(Offset_x<=0)
	{
		Offset_x = x-e_x;
		Err_d = -1;
	}
	if(Offset_x>Offset_y)
	{
		while(x!=e_x)
		{
			if(Offset_k>0)
			{
				y+=1;
				Offset_k += (Offset_y-Offset_x);
			}
			else Offset_k += Offset_y; 
			x+=Err_d;
			LCD501_PutPixel(x,y);
		}	
	}
	else
	{
		while(y!=e_y)
		{
			if(Offset_k>0)
			{
				x+=Err_d;
				Offset_k += (Offset_x-Offset_y);
			}
			else Offset_k += Offset_x;
			y+=1;
			LCD501_PutPixel(x,y);
		}
	}
}
//***************************************************************************//
//����void LCD501_Circle(unsigned int x,unsigned int y,unsigned int r,
//							unsigned int Mode)
//��������Բ����
//������x Ҫ����Բ�ĵ�x����   y Ҫ����Բ�ĵ�y����  r �뾶  Mode�����ģʽ����
//		Mode = M_DOT_CLEAR 0 ��Բ��    Mode = M_DOT_SET 1 ��ʵ��Բ
//������С��	20050521	sunnorth  Mz Design
//ע�⣺��ʵ��Բ��Ҫ�ýϳ�ʱ�䣬�û���Ҫ�����忴�Ź��Ĳ���
//***************************************************************************//
void W_Red_Err(int *a,int *b,int *r);
void LCD_PaintRed_Dot(int x,int y,int a,int b,int mode)
{
    if(mode>0)
    {
       LCD501_Line(x+a,y+b,x-a,y+b);
       LCD501_Line(x+a,y-b,x-a,y-b);
    }
    else
    {
       LCD501_PutPixel(x+a,y+b);
       LCD501_PutPixel(x-a,y+b);
       LCD501_PutPixel(x+a,y-b);
       LCD501_PutPixel(x-a,y-b);
    }
}
void LCD501_Circle(unsigned int x,unsigned int y,unsigned int r,unsigned int Mode)
{
	int arx1=0,ary1,arx2,ary2=0;
	unsigned int uiTemp;
     ary1=r;
     arx2=r;
     uiTemp = LCD501_GetPaintMode();
     LCD501_SetPaintMode(PAINT_COVER);
     while(1)
     {
     	Clear_Watchdog();
        LCD_PaintRed_Dot(x,y,arx1,ary1,Mode);
        W_Red_Err(&arx1,&ary1,&r);
        if(arx1==arx2)
        {
           LCD_PaintRed_Dot(x,y,arx1,ary1,Mode);
           break; 
        }
        LCD_PaintRed_Dot(x,y,arx2,ary2,Mode);
        W_Red_Err(&ary2,&arx2,&r);
        if(arx1==arx2) 
        {
           LCD_PaintRed_Dot(x,y,arx2,ary2,Mode);
           break;
        }
     }
     LCD501_SetPaintMode(uiTemp);
}
//***************************************************************************//
//����void LCD501_Rectangle(unsigned int left, unsigned int top, 
//							unsigned int right, unsigned int bottom, unsigned int Mode)
//�����������γ���
//������left - ���ε����ϽǺ����꣬��Χ0��118
//		top - ���ε����Ͻ������꣬��Χ0��50
//		right - ���ε����½Ǻ����꣬��Χ1��119
//		bottom - ���ε����½������꣬��Χ1��51
//		Mode - ����ģʽ��������������ֵ֮һ��
//				PAINT_HOLLOW:	���ο򣨿��ľ��Σ�
//				PAINT_SOLID:	�����棨ʵ�ľ��Σ�
//������xinqiang	20050521	sunnorth  Mz Design
//ע�⣺��
//***************************************************************************//
void LCD501_Rectangle(unsigned int left, unsigned int top, unsigned int right, unsigned int bottom, unsigned int Mode)
{
	unsigned int uiTemp;
	if(left>right)
	{
		uiTemp = left;
		left = right;
		right = left;
	}
	if(top>bottom)
	{
		uiTemp = top;
		top = bottom;
		bottom = uiTemp;
	}
	if(Mode==PAINT_HOLLOW)
	{
		LCD501_Line(left,top,left,bottom);
		LCD501_Line(left,top,right,top);
		LCD501_Line(right,bottom,left+1,bottom);
		LCD501_Line(right,bottom,right,top+1);
	}
	else
	{
		for(uiTemp=top;uiTemp<=bottom;uiTemp++)
		{
			LCD501_Line(left,uiTemp,right,uiTemp);
			Clear_Watchdog();
		}
	}
}
//***************************************************************************//
//����void LCD501_Bitmap(unsigned int x,unsigned int y,unsigned int *word)
//������OSDȡ�ĺ�����ģ��ʾ����  C���Ե��� 
//������x ��ʾ�ַ�����ʼ�У�0~128��   y Ҫ��ʾ����ģ����ʼ�У�0~64-y_whide��
//		*word Ҫ��ʾ�ĺ�����ģ�������׵�ַ
//������С��	20050519	sunnorth  Mz Design
//ע�⣺ֻ��ʾ��OSDȡ��ģ�����ȡ�ĺ�����ģ����
//***************************************************************************//
unsigned int Shift_Buff[4];
unsigned int Shift_Data_buf[4];
unsigned int Cover_Data_buf[4];
void F_Cover_Initial(unsigned int Number_Bit,unsigned int Number_Shif);
void F_Data_Cover(unsigned int* word,unsigned int S_Number,unsigned int Y_W,unsigned int Y_WW);
void LCD501_Bitmap(unsigned int x,unsigned int y,unsigned int *word)
{
	unsigned int untype,Shift_Num,Read_Back;
	int i,j,m,n;
	unsigned int x_low,x_hight;
	unsigned int y_whide,y_Number,y_Number_8;
	Shift_Num = y&0x07;						//Get the Shift Number
	untype = *(word++);
	i = untype>>8;
	j = (untype&0xff);						//Get the whide
	if((x+j)>128||(y+i)>64||y>64)
		return;
	y = (y>>3)+M_LCD_COL_PAGE;
	x = x+4;
	y_whide = i>>4;							//Get the y whide (16bit one word)
	if(i&0x000f)y_whide+=1;
	//test
	F_Cover_Initial(i,Shift_Num);			//Model the Mask Data
	//test
	y_Number = i+Shift_Num;
	y_Number_8 = y_Number;
	i = y_Number&0x0f;
	y_Number = y_Number>>4;
	if(i)y_Number++;
	i = i&0x07;
	y_Number_8 = y_Number_8>>3;
	if(i&&y_Number_8!=8)y_Number_8++;
	
	for(n=0;n<j;n++)
	{
		x_low = (x&0x000f);				//��λ�е�ַ���õĵ�λָ��
		x_hight = ((x>>4)&0x000f)+0x10;		//��λ�е�ַ���õĸ�λָ��
		x++;
		for(m=0;m<4;m++)Shift_Data_buf[m] = 0;
		for(m=0;m<y_Number_8;m++)
		{
			F_SPLC501_Write_Com(y+m);
			F_SPLC501_Write_Com(x_low);	//���ó�ʼ��ַ 
			F_SPLC501_Write_Com(x_hight);
			Read_Back = F_SPLC501_Read_Data();
			untype = m>>1;
			if(m&0x01)
				Shift_Buff[3-untype] = Shift_Buff[3-untype]|(Read_Back<<8);
			else
				Shift_Buff[3-untype] = Read_Back;
		}
		F_Data_Cover(word,Shift_Num,y_Number,(y_whide<<4));
		for(m=0;m<y_Number_8;m++)
		{
			F_SPLC501_Write_Com(y+m);
			F_SPLC501_Write_Com(x_low);	//���ó�ʼ��ַ 
			F_SPLC501_Write_Com(x_hight);
			untype = m>>1;
			if(m&0x01)
				Read_Back = Shift_Data_buf[3-untype]>>8;// = Shift_Buff[3-untype]|(Read_Back<<8);
			else
				Read_Back = Shift_Data_buf[3-untype];// = Read_Back;
			F_SPLC501_Write_Data(Read_Back);
		}
		word = word+y_whide;
	}
}
//****************************************************************************************//
//����void LCD501_FontSet(unsinged int Font)     [8x16] [6X8]
//����: ѡ����ʾ�ַ��Ĵ�С����
//������Font	�ַ�������ѡ�� 1 8X16   0 6X8
//����: xinqiang		20050519	sunnorth  Mz Design
//ע��: ֻ�����ʾ�����а�����ASCII����ʾ
//*****************************************************************************************//
void LCD501_FontSet(unsigned int Font)
{
	if(Font) CurASCIIFont = 1;
	else CurASCIIFont = 0;
}
//****************************************************************************************//
//����unsigned int LCD501_FontGet(void)     [8x16] [6X8]
//����: ��ȡ��ǰ��ʾ�ַ��Ĵ�С����
//���أ�Font	�ַ�������ѡ�� 1 8X16   0 6X8
//����: xinqiang		20050519	sunnorth  Mz Design
//ע��: ֻ�����ʾ�����а�����ASCII����ʾ
//*****************************************************************************************//
unsigned int LCD501_FontGet(void)
{
	return CurASCIIFont;
}
//****************************************************************************************//
//����void F_LCD_WASII(int x,int page,unsigned int a)     [8x16] [6X8]
//����: ��ʱ8X16��6X8���Դ�ASCII�ַ�����Page��ʾ�����������ж�λ
//������x ��ʾ�ַ�����ʼ�У�0~128��   page ��ʾ�ַ�����ʼҳ��0~6��
//		a ASCII��					  Font	�ַ�������ѡ�� 1 8X16   0 6X8
//����: xinqiang		20050519	sunnorth  Mz Design
//ע��: 
//*****************************************************************************************//
void F_LCD_WASII(int x,int page,unsigned int a)       
{
     int i,j,X_Witch,Y_Witch,y_com;            //�����ݴ�
     int *p_data;
     a -=32; 
     x=x+4;
 
     if(CurASCIIFont)
     {
         X_Witch = 4;
         Y_Witch = 2;
         p_data = &Asii16;                    //�ַ�����ַ
         a=a<<3;
     }
     else
     {
         p_data = &Asii8;                     //�ַ�����ַ
         X_Witch=a<<1;
         a+=X_Witch;
         X_Witch = 3;
         Y_Witch = 1;
     }
     p_data +=a;                               //Ҫд�ַ����׵�ַ
     for(j=0;j<Y_Witch;j++)
     {
		if((page+j)<8)
		{
			y_com = page+0xb0;
			F_SPLC501_Write_Com(y_com+j);
			F_SPLC501_Write_Com(((x&0xf0) >> 4)|0x10);	//���ó�ʼ��ַ 
			F_SPLC501_Write_Com(x&0x0f);
			for(i=0;i<X_Witch;i++,p_data++)
			{   
				if((x+(i<<1))<132)
				{
					F_SPLC501_Write_Data(((*p_data)>>8)&0xff);
					F_SPLC501_Write_Data((*p_data)&0xff);
				}    
            }
        }
    }
}   
//********************************************************************************************//
//����void LCD501_PutChar(int x,int y,unsigned int a)      [8x16] [6X8]
//����: ��ʱ8X16��6X8���Դ�ASCII�ַ��������ж�λ
//������x ��ʾ�ַ�����ʼ�У�0~128��   y ��ʾ�ַ�����ʼ�У�0~63��
//		a ASCII��					 
//����: xinqiang		20050519	sunnorth  Mz Design
//ע��: 
//********************************************************************************************//
void LCD501_PutChar(int x,int y,unsigned int a)       
{
     int i,j,X_Witch,Y_Witch=0,y_com=0;            //�����ݴ�
     unsigned int uiData1,uiData2;
     unsigned int uiMask_low=0x00ff,uiMask = 0x00ff,uiMask_mid = 0x00ff,uiMask_mid2 = 0x00ff;
     unsigned int Read_Back;					//���ص�����
     unsigned int Shift_Num;					//���Ƶĸ���
     int *p_data;
     a -=32; 
     x=x+4;
     Shift_Num = y&0x07;						//ֻȡ����λ
     y = y>>3;//&uiMask_low;
     if(Shift_Num){Y_Witch++;}
     if(CurPaintMode==PAINT_COVER)//||CurPaintMode==PAINT_SET)
     {
		 uiMask = uiMask>>Shift_Num;				//�õ���Ĥ��
		 uiMask_low = (uiMask_low<<(8-Shift_Num))&0x00ff;			//�õ���λ��ģ��
		 uiMask_mid2 = uiMask_low>>(8-Shift_Num);
		 uiMask_mid = uiMask<<Shift_Num;
	}
     if(CurASCIIFont)
     {
         X_Witch = 4;
         Y_Witch = Y_Witch+2;
         p_data = &Asii16;                    //�ַ�����ַ
         a=a<<3;
     }
     else
     {
         p_data = &Asii8;                     //�ַ�����ַ
         X_Witch=a<<1;
         a+=X_Witch;
         X_Witch = 3;
         Y_Witch++;
     }
     p_data +=a;                               //Ҫд�ַ����׵�ַ
     
     for(j=0;j<Y_Witch;j++)
     {
		if((y+j)<8)
		{
			y_com = y+0xb0;
			F_SPLC501_Write_Com(y_com+j);				//���ó�ʼҳ��ַ
			F_SPLC501_Write_Com(((x&0xf0) >> 4)|0x10);	//���ó�ʼ�е�ַ 
			F_SPLC501_Write_Com(x&0x0f);
			F_SPLC501_Write_Com(0xe0);					//����Ϊ��д��ģʽ
			for(i=0;i<X_Witch;i++,p_data++)
			{   
				if((x+(i<<1))<128)
				{
					if(j==0)
					{
						uiData2 = F_SPLC501_Read_Data();
						uiData1 = (*p_data)>>8;
						uiData1 = (uiData1&uiMask)<<Shift_Num;
						uiData2 = (uiData2&uiMask_mid2);//>>Shift_Num;
					}
					else if(j==(Y_Witch-1)&&Shift_Num!=0)
					{
						uiData1 = (*(p_data-X_Witch)>>8)&0x00ff;
						uiData2 = F_SPLC501_Read_Data();
						uiData2 = (uiData2&uiMask_mid);
						uiData1 = (uiData1&uiMask_low)>>(8-Shift_Num);//>>Shift_Num;
					}
					else
					{
						uiData1 = (*(p_data-X_Witch)>>8)&0x00ff;
						uiData2 = ((*p_data)>>8)&0x00ff;
						uiData1 = uiData1>>(8-Shift_Num);
						uiData2 = uiData2<<Shift_Num;
						uiData1 = uiData1|uiData2;
					}
					switch(CurPaintMode)
					{
						case PAINT_CLR: uiData2 = F_SPLC501_Read_Data();Read_Back = (~uiData1)&uiData2;
							break;
						case PAINT_REV: uiData2 = F_SPLC501_Read_Data();Read_Back = uiData1^uiData2;
							break;
						case PAINT_SET: uiData2 = F_SPLC501_Read_Data(); Read_Back = uiData2|uiData1;
							break;
						default: Read_Back = uiData2|uiData1;
							break;
					}
					F_SPLC501_Write_Data(Read_Back);
					if(j==0)
					{
						uiData2 = F_SPLC501_Read_Data();
						uiData1 = *p_data;
						uiData1 = (uiData1&uiMask)<<Shift_Num;
						uiData2 = (uiData2&uiMask_mid2);//>>Shift_Num;
					}
					else if(j==(Y_Witch-1)&&Shift_Num!=0)
					{
						uiData1 = (*(p_data-X_Witch))&0x00ff;
						uiData2 = F_SPLC501_Read_Data();
						uiData2 = (uiData2&uiMask_mid);
						uiData1 = (uiData1&uiMask_low)>>(8-Shift_Num);//>>Shift_Num;
					}
					else
					{
						uiData1 = (*(p_data-X_Witch))&0x00ff;
						uiData2 = (*p_data)&0x00ff;
						uiData1 = uiData1>>(8-Shift_Num);
						uiData2 = uiData2<<Shift_Num;
						uiData1 = uiData1|uiData2;
					}
					switch(CurPaintMode)
					{
						case PAINT_CLR: uiData2 = F_SPLC501_Read_Data();Read_Back = (~uiData1)&uiData2;
							break;
						case PAINT_REV: uiData2 = F_SPLC501_Read_Data();Read_Back = uiData1^uiData2;
							break;
						case PAINT_SET: uiData2 = F_SPLC501_Read_Data();Read_Back = uiData2|uiData1;
							break;
						default: Read_Back = uiData2|uiData1;
							break;
					}
					F_SPLC501_Write_Data(Read_Back);//(*p_data)&0xff);
				}    
            }
            F_SPLC501_Write_Com(0xee);					//����дģʽ����ָ��
        }
    }
}   
//********************************************************************************************//
//����void LCD501_PutString(int x,int y,unsigned int a)      [8x16] [6X8]
//����: ��ʱ8X16��6X8���Դ�ASCII�ַ��������ж�λ
//������x ��ʾ�ַ�����ʼ�У�0~128��   y ��ʾ�ַ�����ʼ�У�0~63��
//		a ASCII��					  
//����: xinqiang		20050519	sunnorth  Mz Design
//ע��: 
//********************************************************************************************//
void LCD501_PutString(int x,int y,unsigned int * string)       
{
	unsigned int x_err=6,y_err=8;
	if(CurASCIIFont)
	{
		x_err = 8;
		y_err = 16;
	}
	while(*string!=0)
	{
		if((x+x_err)>127)
		{
			x = 0;
			y = y+y_err;
		}
		if(y>63) y =0;
		LCD501_PutChar(x,y,*string);
		string++;
		x+=x_err;
	}
}   
