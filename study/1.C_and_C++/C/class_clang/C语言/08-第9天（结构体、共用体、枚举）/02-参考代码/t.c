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
	date_time D;			//时间
	char status;  			//接收状态
	double latitude;   		//纬度
	double longitude; 		 //经度
	char NS;          		//南北极
	char EW;           		//东西
	double speed;      		//速度
}GPS_INFO;

//$GPRMC,024813.640,A,3158.4608,N,11848.3737,E,10.05,324.27,150706,,,A*50

// [0]:$GPRMC
// [1]:024813.640
// [2]:A
// [3]:3158.4608
// [4]:N
// [5]:11848.3737
// [6]:E
// [7]:10.05
// [8]:324.27
// [9]:150706
// [10]:A*50

main()
{
	int i=0;
	char str1[15];
	char str[]="$GPRMC,024813.640,A,3158.4608,N,11848.3737,E,10.05,324.27,150706,,,A*50";
	char *p[13];
	GPS_INFO g;
	p[i] = strtok(str,",");
	while(p[i] != NULL){
	//	printf("[%d]:%s\n",i,p[i]);
		i++;
		p[i] = strtok(NULL,",");
	}
	g.D.year = atoi(p[9]+4)+2000;
	*(p[9]+4) = '\0';
	g.D.month = atoi(p[9]+2);
	*(p[9]+2) = '\0';
	g.D.day = atoi(p[9]);
	printf("%d年%d月%d日\n",g.D.year,g.D.month,g.D.day);
	g.NS = *(p[4]);
	g.longitude = atof(p[3]);
	g.EW = *(p[6]);
	g.latitude = atof(p[5]);
	printf("%s纬%d度%lf分\n%s经%d度%lf分\n",(g.NS=='N')?"北":"南",(int)g.longitude/100,(double)(((int)(g.longitude*10000))%1000000)/10000,(g.EW=='E')?"东":"西",(int)g.latitude/100,(double)(((int)(g.latitude*10000))%1000000)/10000);

}
