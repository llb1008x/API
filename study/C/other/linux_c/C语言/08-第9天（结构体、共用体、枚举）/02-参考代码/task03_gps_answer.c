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
	char time_c[10];//ACSII形式的时间
}date_time;

typedef struct{
	date_time D;						//时间
	char status;  					//接收状态
	double latitude;   		//纬度
	char   latitude_c[11];	//ASCII形式的纬度
	double longitude; 		  //经度
	char   longitude_c[11];//ASCII形式的经度
	char NS;          			//南北极
	char EW;           		//东西
	double speed;      		//速度
	char   speed_c[10];		//ASCII形式的速度
	double high;       		//高度
}GPS_INFO;

//$GPRMC,024813.640,A,3158.4608,N,11848.3737,E,10.05,324.27,150706,,,A*50

main()
{
	int i=0;
	char str1[15];
	char str[]="$GPRMC,024813.640,A,3158.4608,N,11848.3737,E,10.05,324.27,150706,,,A*50";
	char *p[13];
	GPS_INFO g;

	printf("\n");
	puts(str);
	printf("\n\n");

	p[i]=strtok(str,",");				//按","号对字符串进行切割
	while(p[i])
		p[++i]=strtok(NULL,",");

	g.D.year=atoi(*(p+9)+4);			//将第9字段的year提取出来

	strncpy(str1,*(p+9)+2,2);			//提取月
	str1[2]='\0';
	g.D.month=atoi(str1);

	strncpy(str1,*(p+9),2);				//提取日
	str1[2]='\0';
	g.D.day=atoi(str1);

	strncpy(str1,*(p+1),2);				//提取时
	str1[2]='\0';
//	strcpy(g.D.time_c,str1);			
	g.D.hour=atoi(str1)+8;
	*g.D.time_c=atoi(str1)+8;



	strncpy(str1,*(p+1)+2,2);			//提取分
	str1[2]='\0';
//	strcat(g.D.time_c,str1);
	g.D.minute=atoi(str1);

	strncpy(str1,*(p+1)+4,6);			//提取秒
	str1[6]='\0';
//	strcat(g.D.time_c,str1);
	g.D.second=atoi(str1);

	g.latitude=atof(*(p+3));			//提取精度保存为字符串与double
	strcpy(g.latitude_c,*(p+3));
	g.NS=*p[4];

	g.longitude=atof(*(p+5));			//提取维度保存为字符串与double
	strcpy(g.longitude_c,*(p+5));
	g.EW=*p[6];

	printf("gps_parse result:\n\n");		//打印各信息
										//打印时间
	printf("时间:%02d:%02d:%02d\n",g.D.hour,g.D.minute,g.D.second);

	printf("日期:%02d年%02d月%02d日\n",g.D.year,g.D.month,g.D.day);

	strncpy(str1,g.latitude_c,2);		//打印纬度
	*(str1+2)='\0';
	printf("纬度:%c纬%s度%s分\n",g.NS,str1,g.latitude_c+2);

	strncpy(str1,g.longitude_c,3);		//打印经度
	*(str1+3)='\0';
	printf("经度:%c经%s度%s分\n",g.EW,str1,g.longitude_c+3);

}

/*
int gps_deal(struct gps gps_mes,char *message)
*/