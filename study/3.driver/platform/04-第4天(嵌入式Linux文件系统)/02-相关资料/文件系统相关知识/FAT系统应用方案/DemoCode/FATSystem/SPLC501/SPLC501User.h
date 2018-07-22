#ifndef	__LCD_USER_h__
#define	__LCD_USER_h__

//	write your header here
#define Clear_Watchdog()	*(unsigned *)0x7012=1;		// �忴�Ź�
//LCD�����ƽѡ��
#define		M_LCD_VDD_SET	M_LCD_SETR_5		//3.3V����ʱѡ�˶���
#define		M_LCD_VDD		M_LCD_BIAS_9		//....
//#define		M_LCD_VDD_SET	M_LCD_SETR_4	//5.0V����ʱѡ�˶���
//#define		M_LCD_VDD		M_LCD_BIAS_9	//...


//LCD��������
#define		M_LCD_ON		0x00af
#define		M_LCD_OFF		0x00ae
//�����ϵ����ģʽ
#define		M_LCD_POWER_BC	0x002c
#define		M_LCD_POWER_VR	0x002a
#define		M_LCD_POWER_VC	0x0029
#define		M_LCD_POWER_ALL	0x002f
//V5�ڲ���ѹ���ڵ������á���
#define		M_LCD_SETR_0	0x0020
#define		M_LCD_SETR_1	0x0021
#define		M_LCD_SETR_2	0x0022
#define		M_LCD_SETR_3	0x0023
#define		M_LCD_SETR_4	0x0024
#define		M_LCD_SETR_5	0x0025
#define		M_LCD_SETR_6	0x0026
#define		M_LCD_SETR_7	0x0027
//...end
#define		M_LCD_ELE_VOL	0x0081		//��������ģʽ����ʾ���ȣ�
//ƫѹ����
#define		M_LCD_BIAS_9	0x00a2		//V5ʱѡ��ѡ������
#define		M_LCD_BIAS_7	0x00a1		//V3ʱѡ��ѡ������
//Com ɨ�跽ʽ��������
#define		M_LCD_COM_NOR	0x00c0		//������ʽ
#define		M_LCD_COM_REV	0x00c8		//����
//Segment����ѡ��
#define		M_LCD_SEG_NOR	0x00a0		//����
#define		M_LCD_SEG_REV	0x00a1		//����
//ȫ������/�䰵ָ��
#define		M_LCD_ALL_LIGNT	0x00a5		//LCD ALL paint ON
#define		M_LCD_ALL_LOW	0x00a4		//Normal Display mode
//���෴����ʾ����ָ�RAM�����ݲ���
#define		M_LCD_ALL_NOR	0x00a6		//����
#define		M_LCD_ALL_REV	0x00a7		//����
//��ָ̬ʾ������ָ��
#define		M_LCD_STATIC_ON	0x00ad		//ON
#define		M_LCD_STATIC_OFF 0x00ac		//OFF
//������ʾ��ʼ�ж�ӦRAM�к�
#define		M_LCD_BEGIN_LINE 0x0040		//����������ɼӵ�β����Ϊ0~63
//���õ�ǰҳ����
#define		M_LCD_COL_PAGE	0x00b0		//����ָ���ɼ�β��0~8
//���õ�ǰ�л���
#define		M_LCD_COL_LINE_LOW	0x0004		//����ָ�����λ��Ч
#define		M_LCD_COL_LINE_HIG	0x0010		//����ָ�����λ��Ч
//========================================================================//
//API�ӿں����궨��
//========================================================================//
/////-----       ��Ļ�ߴ�	    -----/////
#define SCR_MAX_COL		128	
#define SCR_MAX_ROW		64						
//#define TEXT_MAX_COL	20
//#define TEXT_MAX_ROW	4

/////-----       ��Դģʽ	    -----/////
#define PWR_ON			0x0001					
#define	PWR_OFF			0x0000
#define PWR_SLEEP		0x0002
#define PWR_STDBY		0x0003

/////-----       ͼ����ʾģʽ	    -----/////
#define PAINT_COVER		0
#define PAINT_SET		1
#define PAINT_CLR		2
#define PAINT_REV		3
#define PAINT_RECO		4

#define PAINT_HOLLOW	0
#define PAINT_SOLID		1	
void LCD501_Init(unsigned int InitialData);
void LCD501_Power(unsigned int ModeCode);
void LCD501_SetContrast(unsigned int Contrast);
unsigned int LCD501_GetContrast(void);
void LCD501_ScrollUp(unsigned int Rows);
unsigned int LCD501_GetScroll(void);
void LCD501_ReverseColor(void);
void LCD501_ClrScreen(unsigned int Mode);
void LCD501_SetPaintMode(unsigned int ModeCode);
unsigned int LCD501_GetPaintMode(void);
void LCD501_PutPixel(unsigned int x,unsigned int y);
void LCD501_Line(unsigned int x,unsigned int y,unsigned int e_x,unsigned int e_y);
void LCD501_Circle(unsigned int x,unsigned int y,unsigned int r,unsigned int Mode);
void LCD501_Rectangle(unsigned int left, unsigned int top, unsigned int right, unsigned int bottom, unsigned int Mode);
void LCD501_Bitmap(unsigned int x,unsigned int y,unsigned int *word);
void LCD501_FontSet(unsigned int Font);
unsigned int LCD501_FontGet(void);
void LCD501_PutChar(int x,int y,unsigned int a);
void LCD501_PutString(int x,int y,unsigned int * string);

#endif
