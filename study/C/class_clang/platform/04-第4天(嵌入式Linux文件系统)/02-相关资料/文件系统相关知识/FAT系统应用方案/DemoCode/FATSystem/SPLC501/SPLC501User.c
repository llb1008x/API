//***************************************************************************//
//文件描述：SPLC501用户接口驱动程序，
//
//声明：	小丑	20050519	Sunnorth	Mz Design
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
unsigned int CurLCDSet;					//Show the LCD Set灰度
unsigned int CurScroll;					//Show the scroll dotes
unsigned int CurScrMode;				//显示正相/反相（1/0）
unsigned int CurASCIIFont;				//当前ASCII码的字号（8X16/6X8）

//***************************************************************************//
//程序：F_LCD_ON_OFF(unsigned int SetCom)
//描述：液晶开关函数  C语言调用 
//参数：unsigned int SetCom		SetCom=0:LCD Off  SetCom!=0:LCD On
//声明：小丑	20050519	sunnorth  Mz Design
//注意：破坏R1寄存器，保护R1
//***************************************************************************//
void F_LCD_ONOFF(unsigned int SetCom)
{
	if(SetCom)							//ON LCD
		F_SPLC501_Write_Com(M_LCD_ON);
	else								//OFF LCD
		F_SPLC501_Write_Com(M_LCD_OFF);
}
//***************************************************************************//
//程序：LCD501_Init(unsigned int InitialData)
//描述：液晶显示初始化函数  C语言调用 
//参数：unsigned int InitialData	初始化显示的字
//声明：xinqiang	20050519	sunnorth  Mz Design
//注意：破坏R1寄存器，保护R1
//***************************************************************************//
void LCD501_Init(unsigned int InitialData)
{
	unsigned int i,j;
	F_SPLC501_Initial_IO();	//Initial LCD IO
	F_SPLC501_Write_Com(M_LCD_ON);							//LCD On
	F_SPLC501_Write_Com(M_LCD_POWER_ALL);		//设置上电控制模式
	
	F_SPLC501_Write_Com(M_LCD_ELE_VOL);			//电量设置模式（显示亮度）
	F_SPLC501_Write_Com(0x0012);				//指令数据0x0000~0x003f
	
	F_SPLC501_Write_Com(M_LCD_VDD_SET);			//V5内部电压调节电阻设置
	F_SPLC501_Write_Com(M_LCD_VDD);			//LCD偏压设置，V3时选
	
	F_SPLC501_Write_Com(M_LCD_COM_NOR);			//Com 扫描方式设置
	F_SPLC501_Write_Com(M_LCD_SEG_REV);			//Segment方向选择
	F_SPLC501_Write_Com(M_LCD_ALL_LOW);			//全屏点亮/变暗指令
	F_SPLC501_Write_Com(M_LCD_ALL_NOR);			//正向反向显示控制指令
	
	F_SPLC501_Write_Com(M_LCD_STATIC_OFF);		//关闭静态指示器
	F_SPLC501_Write_Com(0x0000);				//指令数据
	
//	F_SPLC501_Write_Com(0x00ee);				//读写改模式结束指令
	F_SPLC501_Write_Com(M_LCD_BEGIN_LINE+0);	//设置显示起始行对应RAM
	
	for(i=0;i<8;i++)							//往LCD中填充初始化的显示数据
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
// 程序：	void LCD501_Power(unsigned int ModeCode)
// 描述：	SPLC501电源管理,进入参数指定的模式，而当LCD在SLEEP和STDBY模式下时，
//			调用该函数会使LCD唤醒
// 参数：		ModeCode - 可以是下列参数之一：
//				1	PWR_ON: 	正常工作模式
//				2	PWR_SLEEP:	睡眠模式，屏幕不显示，但内容在唤醒后仍保留
//				3	PWR_STDBY: 待机模式，比SLEEP省电，但显示内容会丢失
//				0	PWR_OFF:	关闭电源
// 声明：	xinqiang	20050624	sunnorth  Mz Design
// 注意：		无
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
// 程序：	void LCD501_SetContrast(unsigned int Contrast)
// 描述：	设置显示对比度   C语言调用
// 参数：	Contrast - 对比度值，范围从0到31，数值越大对比度越高
// 返回：	无
// 声明：	xinqiang	20050624	Mz Design
// 注意：   无
//***************************************************************************//
void LCD501_SetContrast(unsigned int Contrast)
{
	if(Contrast>0x003f)
		Contrast = 0x003f;
	F_SPLC501_Write_Com(M_LCD_ELE_VOL);			//电量设置模式（显示亮度）
	F_SPLC501_Write_Com(Contrast);			//电量设置模式（显示亮度）
	CurLCDSet = Contrast;
}
//***************************************************************************//
// 程序：	unsigned int LCD501_GetContrast(void)
// 描述：	获取显示对比度   C语言调用
// 参数：	无
// 返回：	当前显示的对比度值
// 声明：	xinqiang	20050624	Mz Design
// 注意：   无
//***************************************************************************//
unsigned int LCD501_GetContrast(void)
{
	return CurLCDSet;//
}
//***************************************************************************//
// 程序：	void LCD501_ScrollUp(unsigned int Rows)
// 描述：	屏幕向上滚动   C语言调用
// 参数：	Rows  屏幕向上滚动的像素数，范围从1到64
// 返回：	null
// 声明：	xinqiang	20050624	Mz Design
// 注意：   无
//***************************************************************************//
void LCD501_ScrollUp(unsigned int Rows)
{
	CurScroll = CurScroll+Rows;
	while(CurScroll>=SCR_MAX_ROW)
		CurScroll -= SCR_MAX_ROW;
	F_SPLC501_Write_Com(M_LCD_BEGIN_LINE+CurScroll);	//设置显示起始行对应RAM
}
//***************************************************************************//
// 程序：	unsigned int LCD501_GetScroll(void)
// 描述：	获取屏幕已经向上滚动的值   C语言调用
// 参数：	无
// 返回：	屏幕已经向上滚动的值
// 声明：	xinqiang	20050624	Mz Design
// 注意：   无
//***************************************************************************//
unsigned int LCD501_GetScroll(void)
{
	return CurScroll;//
}
//***************************************************************************//
// 程序：	void LCD501_ReverseColor(void)
// 描述：	屏幕反色显示。每执行一次该函数，屏幕颜色翻转一次。   C语言调用
// 参数：	无
// 返回：	无
// 声明：	xinqiang	20050624	Mz Design
// 注意：   无
//***************************************************************************//
void LCD501_ReverseColor(void)
{
	if(CurScrMode)//
	{
		F_SPLC501_Write_Com(M_LCD_ALL_REV);			//正向反向显示控制指令
		CurScrMode = 0;
	}
	else
	{
		F_SPLC501_Write_Com(M_LCD_ALL_NOR);			//正向反向显示控制指令
		CurScrMode = 1;
	}
}
//***************************************************************************//
//程序：LCD501_ClrScreen(unsigned int Mode)
//描述：液晶整屏清屏函数  C语言调用 
//参数：unsigned int Mode	清屏类型Mode=1 则全点亮，Mode=0 内全清零
//声明：小丑	20050519	sunnorth  Mz Design
//注意：破坏R1寄存器，保护R1
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
		F_SPLC501_Write_Com(0xee);					//读改写模式结束指令
		for(j=0;j<128;j++)
		{
			F_SPLC501_Write_Data(Mode);
		}
	}
}
//***************************************************************************//
//程序：F_LCD_LineRun(unsigned int line)
//描述：液晶显示起始行设置函数  C语言调用  （滚屏功能用） 
//参数：unsigned int line	起始RAM行，0~63
//声明：小丑	20050527	sunnorth  Mz Design
//注意：破坏R1寄存器，保护R1
//***************************************************************************//
void F_LCD_LineRun(unsigned int line)
{
	if(line>=64)
		line = 0;
	F_SPLC501_Write_Com(M_LCD_BEGIN_LINE+line);	//设置显示起始行对应RAM
}
//***************************************************************************//
//程序：void F_LCD_WOSCASII(unsigned int x,unsigned int page,unsigned int d)
//描述：OSD取的ASCII字符显示函数  C语言调用 
//参数：x 显示字符的起始列（0~128）   page 显示字符的起始页（0~6）
//		d ASCII码
//声明：小丑	20050519	sunnorth  Mz Design
//注意：仅用于显示用OSD取字模工具取的16X16、16X12字模的ASCII字符
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
	x_low = (x&0x000f)+ M_LCD_COL_LINE_LOW;				//定位列地址设置的低位指令
	x_hight = ((x>>4)&0x000f)+M_LCD_COL_LINE_HIG;		//定位列地址设置的高位指令
		
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
// 程序：	void LCD501_SetPaintMode(unsigned int ModeCode)
// 描述：	设置图形显示模式	
// 参数：	ModeCode - 图形显示模式，可以是下列数值之一：
//			0	PAINT_COVER:	覆盖显示。图形所在位置的原有显示内容将被覆盖掉
//			1	PAINT_SET:		叠加显示。图形所在位置中原有的黑色点将被保留
//			2	PAINT_CLR:		擦除显示。图形反色显示，但图形所在位置中原有的白
//								色点将被保留
//			3	PAINT_REV:		异或显示。图形中每一个点与原有的点做异或，即如果
//								图形中某点的颜色与其所在位置原有的颜色相同，则
//								该点显示白色，如果新颜色与原来颜色不同，则该点
//								显示黑色
//			4	PAINT_RECO      取反并覆盖显示。原始图像数据取反，然后图形所在位置
//								的原有显示内容将被覆盖掉
// 返回：		无
// 声明：	xinqiang	20050810	Mz Design
// 注意：
//***************************************************************************//
void LCD501_SetPaintMode(unsigned int ModeCode)
{
	if(ModeCode > 4)
		ModeCode = PAINT_COVER;
	CurPaintMode = ModeCode;
}
//***************************************************************************//
// 程序：	unsigned int LCD501_GetPaintMode(void)
// 描述：	设置图形显示模式
// 参数：   无	
// 返回：	ModeCode - 图形显示模式，可以是下列数值之一：
//			0	PAINT_COVER:	覆盖显示。图形所在位置的原有显示内容将被覆盖掉
//			1	PAINT_SET:		叠加显示。图形所在位置中原有的黑色点将被保留
//			2	PAINT_CLR:		擦除显示。图形反色显示，但图形所在位置中原有的白
//								色点将被保留
//			3	PAINT_REV:		异或显示。图形中每一个点与原有的点做异或，即如果
//								图形中某点的颜色与其所在位置原有的颜色相同，则
//								该点显示白色，如果新颜色与原来颜色不同，则该点
//								显示黑色
//			4	PAINT_RECO      取反并覆盖显示。原始图像数据取反，然后图形所在位置
//								的原有显示内容将被覆盖掉
// 声明：	xinqiang	20050810	Mz Design
// 注意：
//***************************************************************************//
unsigned int LCD501_GetPaintMode(void)
{
	return CurPaintMode;
}
//***************************************************************************//
//程序：void LCD501_PutPixel(unsigned int x,unsigned int y)
//描述：画点子程序
//参数：x 要画点的x坐标   y 要画的点的y坐标  
//返回：无
//声明：xinqiang	20050521	sunnorth  Mz Design
//注意：无
//***************************************************************************//
void LCD501_PutPixel(unsigned int x,unsigned int y)
{
	unsigned int x_low,x_hight;			//定义列地址的高低位指令
	unsigned int Dot_Mask_Buf=0x0001;
	unsigned int y_Page;			//用于存放要画点的位置所在的byte数据位置
	x = x+4;
	x_low = (x&0x000f);					//定位列地址设置的低位指令
	x_hight = ((x>>4)&0x000f)+0x10;		//定位列地址设置的高位指令
	y_Page = y&0x07;					//取低三位
	while(y_Page!=0)
	{
		Dot_Mask_Buf = Dot_Mask_Buf<<1;
		y_Page--;
	}
	y_Page = y>>3;						//Get the page of the byte
	F_SPLC501_Write_Com(M_LCD_COL_PAGE+y_Page);
	F_SPLC501_Write_Com(x_low);
	F_SPLC501_Write_Com(x_hight);
	F_SPLC501_Write_Com(0xe0);					//设置为读写改模式
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
	F_SPLC501_Write_Com(0xee);					//读写改模式结束
}
//***************************************************************************//
//程序：void LCD501_Line(unsigned int x,unsigned int y,unsigned int e_x,
//							unsigned int e_y)
//描述：画直线程序
//参数：x 要画线的x起点坐标   		 y 要画的线的y起点坐标  
//		e_x 要画线的x 终点坐标  	 e_y 要画的线的y起点坐标
//返回：无
//声明：小丑	200505121	sunnorth  Mz Design
//注意：
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
//程序：void LCD501_Circle(unsigned int x,unsigned int y,unsigned int r,
//							unsigned int Mode)
//描述：画圆程序
//参数：x 要画的圆心的x坐标   y 要画的圆心的y坐标  r 半径  Mode：绘点模式……
//		Mode = M_DOT_CLEAR 0 画圆框    Mode = M_DOT_SET 1 画实心圆
//声明：小丑	20050521	sunnorth  Mz Design
//注意：画实心圆需要用较长时间，用户需要做好清看门狗的操作
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
//程序：void LCD501_Rectangle(unsigned int left, unsigned int top, 
//							unsigned int right, unsigned int bottom, unsigned int Mode)
//描述：画矩形程序
//参数：left - 矩形的左上角横坐标，范围0到118
//		top - 矩形的左上角纵坐标，范围0到50
//		right - 矩形的右下角横坐标，范围1到119
//		bottom - 矩形的右下角纵坐标，范围1到51
//		Mode - 绘制模式，可以是下列数值之一：
//				PAINT_HOLLOW:	矩形框（空心矩形）
//				PAINT_SOLID:	矩形面（实心矩形）
//声明：xinqiang	20050521	sunnorth  Mz Design
//注意：无
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
//程序：void LCD501_Bitmap(unsigned int x,unsigned int y,unsigned int *word)
//描述：OSD取的汉字字模显示函数  C语言调用 
//参数：x 显示字符的起始列（0~128）   y 要显示的字模的起始行（0~64-y_whide）
//		*word 要显示的汉字字模的数据首地址
//声明：小丑	20050519	sunnorth  Mz Design
//注意：只显示用OSD取字模软件所取的汉字字模数据
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
		x_low = (x&0x000f);				//定位列地址设置的低位指令
		x_hight = ((x>>4)&0x000f)+0x10;		//定位列地址设置的高位指令
		x++;
		for(m=0;m<4;m++)Shift_Data_buf[m] = 0;
		for(m=0;m<y_Number_8;m++)
		{
			F_SPLC501_Write_Com(y+m);
			F_SPLC501_Write_Com(x_low);	//设置初始地址 
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
			F_SPLC501_Write_Com(x_low);	//设置初始地址 
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
//程序：void LCD501_FontSet(unsinged int Font)     [8x16] [6X8]
//描述: 选择显示字符的大小类型
//参数：Font	字符的类型选择 1 8X16   0 6X8
//声明: xinqiang		20050519	sunnorth  Mz Design
//注意: 只针对显示驱动中包含的ASCII码显示
//*****************************************************************************************//
void LCD501_FontSet(unsigned int Font)
{
	if(Font) CurASCIIFont = 1;
	else CurASCIIFont = 0;
}
//****************************************************************************************//
//程序：unsigned int LCD501_FontGet(void)     [8x16] [6X8]
//描述: 获取当前显示字符的大小类型
//返回：Font	字符的类型选择 1 8X16   0 6X8
//声明: xinqiang		20050519	sunnorth  Mz Design
//注意: 只针对显示驱动中包含的ASCII码显示
//*****************************************************************************************//
unsigned int LCD501_FontGet(void)
{
	return CurASCIIFont;
}
//****************************************************************************************//
//程序：void F_LCD_WASII(int x,int page,unsigned int a)     [8x16] [6X8]
//描述: 显时8X16、6X8的自带ASCII字符，定Page显示，不能任意行定位
//参数：x 显示字符的起始列（0~128）   page 显示字符的起始页（0~6）
//		a ASCII码					  Font	字符的类型选择 1 8X16   0 6X8
//声明: xinqiang		20050519	sunnorth  Mz Design
//注意: 
//*****************************************************************************************//
void F_LCD_WASII(int x,int page,unsigned int a)       
{
     int i,j,X_Witch,Y_Witch,y_com;            //数据暂存
     int *p_data;
     a -=32; 
     x=x+4;
 
     if(CurASCIIFont)
     {
         X_Witch = 4;
         Y_Witch = 2;
         p_data = &Asii16;                    //字符表首址
         a=a<<3;
     }
     else
     {
         p_data = &Asii8;                     //字符表首址
         X_Witch=a<<1;
         a+=X_Witch;
         X_Witch = 3;
         Y_Witch = 1;
     }
     p_data +=a;                               //要写字符的首地址
     for(j=0;j<Y_Witch;j++)
     {
		if((page+j)<8)
		{
			y_com = page+0xb0;
			F_SPLC501_Write_Com(y_com+j);
			F_SPLC501_Write_Com(((x&0xf0) >> 4)|0x10);	//设置初始地址 
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
//程序：void LCD501_PutChar(int x,int y,unsigned int a)      [8x16] [6X8]
//描述: 显时8X16、6X8的自带ASCII字符，任意行定位
//参数：x 显示字符的起始列（0~128）   y 显示字符的起始行（0~63）
//		a ASCII码					 
//声明: xinqiang		20050519	sunnorth  Mz Design
//注意: 
//********************************************************************************************//
void LCD501_PutChar(int x,int y,unsigned int a)       
{
     int i,j,X_Witch,Y_Witch=0,y_com=0;            //数据暂存
     unsigned int uiData1,uiData2;
     unsigned int uiMask_low=0x00ff,uiMask = 0x00ff,uiMask_mid = 0x00ff,uiMask_mid2 = 0x00ff;
     unsigned int Read_Back;					//读回的数据
     unsigned int Shift_Num;					//左移的个数
     int *p_data;
     a -=32; 
     x=x+4;
     Shift_Num = y&0x07;						//只取低三位
     y = y>>3;//&uiMask_low;
     if(Shift_Num){Y_Witch++;}
     if(CurPaintMode==PAINT_COVER)//||CurPaintMode==PAINT_SET)
     {
		 uiMask = uiMask>>Shift_Num;				//得到掩膜数
		 uiMask_low = (uiMask_low<<(8-Shift_Num))&0x00ff;			//得到低位掩模数
		 uiMask_mid2 = uiMask_low>>(8-Shift_Num);
		 uiMask_mid = uiMask<<Shift_Num;
	}
     if(CurASCIIFont)
     {
         X_Witch = 4;
         Y_Witch = Y_Witch+2;
         p_data = &Asii16;                    //字符表首址
         a=a<<3;
     }
     else
     {
         p_data = &Asii8;                     //字符表首址
         X_Witch=a<<1;
         a+=X_Witch;
         X_Witch = 3;
         Y_Witch++;
     }
     p_data +=a;                               //要写字符的首地址
     
     for(j=0;j<Y_Witch;j++)
     {
		if((y+j)<8)
		{
			y_com = y+0xb0;
			F_SPLC501_Write_Com(y_com+j);				//设置初始页地址
			F_SPLC501_Write_Com(((x&0xf0) >> 4)|0x10);	//设置初始列地址 
			F_SPLC501_Write_Com(x&0x0f);
			F_SPLC501_Write_Com(0xe0);					//设置为读写改模式
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
            F_SPLC501_Write_Com(0xee);					//读改写模式结束指令
        }
    }
}   
//********************************************************************************************//
//程序：void LCD501_PutString(int x,int y,unsigned int a)      [8x16] [6X8]
//描述: 显时8X16、6X8的自带ASCII字符，任意行定位
//参数：x 显示字符的起始列（0~128）   y 显示字符的起始行（0~63）
//		a ASCII码					  
//声明: xinqiang		20050519	sunnorth  Mz Design
//注意: 
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
