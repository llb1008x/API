
/*=========================================================================
  �������ƣ�	5_ts_calibrate  
  ����ļ���	main.c ,lcd_lib.a
  ���������� 	��ģ��У׼�����ڹ��ղ��죬����Ӱ��ȵ��·�����������
  ά����¼��	2010-08-24 v1.1		add by dxh
=========================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <pthread.h>
#include <semaphore.h>

#include "TFT_API.h"
#include "TimerTick.h"

#define SCR_WIDTH        320
#define SCR_HEIGHT       240
#define RADIUS_FRINGE    5	//У׼Բ���봥ģ�߽�ֵ
#define CIRCLE_RADIUS    10 //�뾶
#define SCR_FRINGE       (RADIUS_FRINGE+CIRCLE_RADIUS)	//Ӧ���߽綨���Բ�㵽���ϵ�ֵ
#define CIRCLE_X1        SCR_FRINGE	                //��1����Բ������
#define CIRCLE_Y1        SCR_FRINGE	                //��1����Բ������
#define CIRCLE_X2        SCR_FRINGE	                //��2����Բ������
#define CIRCLE_Y2        (SCR_HEIGHT-SCR_FRINGE)	//��2����Բ������
#define CIRCLE_X3        SCR_WIDTH-SCR_FRINGE	    //��3����Բ������
#define CIRCLE_Y3        SCR_HEIGHT-SCR_FRINGE	    //��3����Բ������
#define CIRCLE_X4        SCR_WIDTH-SCR_FRINGE	    //��4����Բ������
#define CIRCLE_Y4        SCR_FRINGE	                //��4����Բ������
#define CIRCLE_X5        (SCR_WIDTH >> 1)	        //��5����Բ������
#define CIRCLE_Y5        (SCR_HEIGHT >> 1)	        //��5����Բ������

#define START_XY(x)       ((x)-CIRCLE_RADIUS) //������ʼ����
#define END_XY(x)         ((x)+CIRCLE_RADIUS) //���߽�������
    
 struct TS_RET{
  unsigned short pressure;
  unsigned short x;
  unsigned short y;
  unsigned short pad;
} ts;
static int touch_calibrate_lb = 1023;// ���Ե��Ӧ��Ĭ��ADת��ֵ(16bit)
static int touch_calibrate_rb = 0;// �ұ�Ե��Ӧ��Ĭ��ADת��ֵ(16bit)
static int touch_calibrate_tb = 0;// �ϱ�Ե��Ӧ��Ĭ��ADת��ֵ(16bit)
static int touch_calibrate_bb = 1023;// �±�Ե��Ӧ��Ĭ��ADת��ֵ(16bit)		
static int ca_tp[5][2];	//�������ADֵ
static int ts_fd;
WIN_HANDLE  MainFrame;

int draw_line(unsigned int cx,unsigned int cy)
{
	TFT_Line(MainFrame, START_XY(cx), cy, END_XY(cx), cy);
	TFT_Line(MainFrame, cx, START_XY(cy), cx, END_XY(cy));
}

int draw_dot(unsigned int num)
{
	printf("\nTFT_ClearWindow\n");
	TFT_ClearWindow(MainFrame);
	switch(num)
	{
		case 1:
			TFT_Circle(MainFrame, CIRCLE_X1,CIRCLE_Y1,CIRCLE_RADIUS,1);
			draw_line(CIRCLE_X1,CIRCLE_Y1);
			break;
		case 2:
			TFT_Circle(MainFrame, CIRCLE_X2,CIRCLE_Y2,CIRCLE_RADIUS,1);
			draw_line(CIRCLE_X2,CIRCLE_Y2);
			break;
		case 3:
			TFT_Circle(MainFrame, CIRCLE_X3,CIRCLE_Y3,CIRCLE_RADIUS,1);
			draw_line(CIRCLE_X3,CIRCLE_Y3);
			break;	
		case 4: 
			TFT_Circle(MainFrame, CIRCLE_X4,CIRCLE_Y4,CIRCLE_RADIUS,1);
			draw_line(CIRCLE_X4,CIRCLE_Y4);
			break;			
		case 5: 
			TFT_Circle(MainFrame, CIRCLE_X5,CIRCLE_Y5,CIRCLE_RADIUS,1);
			draw_line(CIRCLE_X5,CIRCLE_Y5);
			break;		
	}
	return 0;
}

int ts_read()
{
	int ret = -1;
	int temp_x,temp_y;
	
	while(ret < 0)
	{
		ret = read(ts_fd,&ts,sizeof(ts));
	}	
	temp_x = ts.x;
	temp_y = ts.y;
	ts.x = SCR_WIDTH*(touch_calibrate_lb - temp_x)/(touch_calibrate_lb - touch_calibrate_rb);
	ts.y = SCR_HEIGHT*(temp_y - touch_calibrate_tb)/(touch_calibrate_bb - touch_calibrate_tb);	
	
	return ret;
}
int ts_calibrate_read()
{
	int ret = -1;
	while(ret < 0)
	{
		ret = read(ts_fd,&ts,sizeof(ts));
	}	
	return ret;
}
int ts_calibrate()
{	
	int tb,bb,lb,rb; //4��У׼��ɵ�AD��ƽ��ֵ��LCD�޹�
	//������LCD�������ʵ�ʱ߽�ADֵ:���ϵĵ㵽����͵�ADֵ���Լ������ұߵ�ADֵ
	int tb_fringe,lr_fringe;   
	
	draw_dot(1);
	
	if(ts_calibrate_read()<0)
		printf("read ts error\n");
	else		
	{
		ca_tp[0][0] = ts.x;
		ca_tp[0][1] = ts.y;
		printf("press x1 is %d\n",ts.x);
		printf("press y1 is %d\n",ts.y);	
		draw_dot(2);
	}
		
	if(ts_calibrate_read()<0)
		printf("read ts error\n");
	else		
	{
		ca_tp[1][0] = ts.x;
		ca_tp[1][1] = ts.y;	
		printf("press x2 is %d\n",ts.x);
		printf("press y2 is %d\n",ts.y);		
		draw_dot(3);
	}		
	
	if(ts_calibrate_read()<0)
		printf("read ts error\n");
	else		
	{
		ca_tp[2][0] = ts.x;
		ca_tp[2][1] = ts.y;	
		printf("press x3 is %d\n",ts.x);
		printf("press y3 is %d\n",ts.y);		
		draw_dot(4);
	}		
	
	if(ts_calibrate_read()<0)
		printf("read ts error\n");
	else		
	{
		ca_tp[3][0] = ts.x;
		ca_tp[3][1] = ts.y;	
		printf("press x4 is %d\n",ts.x);
		printf("press y4 is %d\n",ts.y);		
		draw_dot(5);
	}	
	
	if(ts_calibrate_read()<0)
		printf("read ts error\n");
	else		
	{
		ca_tp[4][0] = ts.x;
		ca_tp[4][1] = ts.y;	
		printf("press x5 is %d\n",ts.x);
		printf("press y5 is %d\n",ts.y);	
		
		//��ʱ����У׼ȡ4����Ľ�����0~4 ��һ����ƽ��ֵ
		tb = (ca_tp[0][1]+ca_tp[3][1])/2;
		bb = (ca_tp[1][1]+ca_tp[2][1])/2; 
		lb = (ca_tp[0][0]+ca_tp[1][0])/2;
		rb = (ca_tp[2][0]+ca_tp[3][0])/2;
		printf("\ntb is %d\n",tb);
		printf("bb is %d\n",bb);
		printf("lb is %d\n",lb);		
		printf("rb is %d\n",rb);
		
		//����������ؼ�AD���ٳ���SCR_FRINGE�õ��㵽�߽��ADֵ(�ڶ�����ƽ��ֵ)
		tb_fringe = (SCR_FRINGE*(bb-tb))/(240-SCR_FRINGE*2);
		lr_fringe = (SCR_FRINGE*(lb-rb))/(320-SCR_FRINGE*2);
		printf("tb_fringe is %d\n",tb_fringe);		
		printf("lr_fringe is %d\n",lr_fringe);		
		
		//�Ӽ��߽�ֵ���õ���ʼֵ
		//��tb:����LCD�޹�ֱ�ӵ���õ���ADֵ��ȥ�õ���LCD�����󵽱߽�ʵ�ʵ�ADֵ��
		//�Ӷ��õ�ֱ�ӵ������LCD��������ʼ�߽�ֵ��������������������Ϊ0		
		touch_calibrate_tb = tb - tb_fringe;
		touch_calibrate_bb = bb + tb_fringe;//����õ��Ľ������Ϲ����󵽵ͱߵ�ֵ�õ��ͱ߽�ֵ
		touch_calibrate_lb = lb + lr_fringe;//���ϵ���ߵ�ֵ�õ���߽�ֵ
		touch_calibrate_rb = rb - lr_fringe;//��ȥ�ұߵ�ֵ�õ��ұ߽�ֵ	
		printf("\ntouch_calibrate_tb is %d\n",touch_calibrate_tb);
		printf("touch_calibrate_bb is %d\n",touch_calibrate_bb);
		printf("touch_calibrate_lb is %d\n",touch_calibrate_lb);		
		printf("touch_calibrate_rb is %d\n",touch_calibrate_rb);		

		return 0;
	}	
}

int main(void)
{
	int ret;
	char *fbname = "/dev/fb0";
	
	getFBP(fbname);
	TFT_Init();	
	//touch_init();
	//TimerTickInit();
	MainFrame = TFT_CreateWindowEx(0, 0, 320, 240, COLOR_WHITE);
	TFT_SetColor(MainFrame, COLOR_RED);
	
	ts_fd = open("/dev/touch_driver",O_RDWR);
	if(ts_fd<0)
			printf("open /dev/touch_driver error");
			
	ts_calibrate(); //У׼����
	TFT_ClearWindow(MainFrame);
	TFT_SetColor(MainFrame, COLOR_BLACK);
	while(1)
	{
			ret = ts_read();
			if(ret<0)
					continue;                       
			if(ts.pressure == 0)
				printf("\n press up:\n");
			else if(ts.pressure == 1)
				printf("\n press down:\n");  
				
			printf("\n         x is %d\n",ts.x);
			printf("         y is %d\n",ts.y);
			TFT_ClearWindow(MainFrame);
			TFT_Circle(MainFrame, ts.x,ts.y,10,0);
	}		
	return 0;
}




