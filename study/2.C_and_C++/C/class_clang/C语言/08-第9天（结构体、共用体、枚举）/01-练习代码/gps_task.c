#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct{
	int year;  
	int month; 
	int day;
	int hour;
	int minute;
	int second;
}date_time;

typedef struct{
	date_time D;			//ʱ��
	char status;  			//����״̬
	double latitude;   		//γ��
	double longitude; 		 //����
	char NS;          		//�ϱ���
	char EW;           		//����
	double speed;      		//�ٶ�
}GPS_INFO;

//$GPRMC,024813.640,A,3158.4608,N,11848.3737,E,10.05,324.27,150706,,,A*50

main()
{
	int i=0;
	char str1[15];
	char str[]="$GPRMC,024813.640,A,3158.4608,N,11848.3737,E,10.05,324.27,150706,,,A*50";
	char *p[13];
	GPS_INFO g;


}
