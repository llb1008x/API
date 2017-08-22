/*************************************************************************
	> File Name: test_gps.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年08月22日 星期二 20时07分42秒


题目：	
3.了解GPS相关知识
    $GPRMC协议格式
　　例：$GPRMC,024813.640,A,3158.4608,N,11848.3737,E,10.05,324.27,150706,,,A*50
　　字段0：$GPRMC，语句ID，表明该语句为Recommended Minimum Specific GPS/TRANSIT Data（RMC）推荐最小定位信息
　　字段1：UTC时间，hhmmss.sss格式(UTC时间是世界统一时间，中国的时间是UTC+8，比如：上面的时间是02:48:13而中国的时间是10:48:13)
　　字段2：状态，A=定位，V=未定位
　　字段3：纬度ddmm.mmmm，度分格式（前导位数不足则补0） 
　　字段4：纬度N（北纬）或S（南纬）
　　字段5：经度dddmm.mmmm，度分格式（前导位数不足则补0）
　　字段6：经度E（东经）或W（西经）
　　字段7：速度，节，Knots
　　字段8：方位角，度
　　字段9：UTC日期，DDMMYY格式
　　字段10：磁偏角，（000 - 180）度（前导位数不足则补0）
　　字段11：磁偏角方向，E=东W=西
　　字段16：校验值

提示：利用strtok切割、sscanf或者strncpy、atoi、atof	
	
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>



//$GPRMC,024813.640,A,3158.4608,N,11848.3737,E,10.05,324.27,150706,,,A*50

//定义时间相关信息
typedef struct time_clock 
{
	int year;
	int month;
	int day;
	int hours;
	int minute;
	int second;
}date_time;


//定义GPS其他信息
typedef struct{
	date_time D;			//时间
	char status;  			//接收状态
	double latitude;   		//纬度
	char   latitude_c[11];	//ASCII形式的纬度
	double longitude; 		//经度
	char   longitude_c[11];	//ASCII形式的经度
	char NS;          		//南北极
	char EW;           		//东西
	double speed;      		//速度
	char   speed_c[10];		//ASCII形式的速度
	double high;       		//高度
}GPS_INFO;


int main()
{
//1.定义相关的量
	char str[]="$GPRMC,024813.640,A,3158.4608,N,11848.3737,E,10.05,324.27,150706,,,A*50";
	char str1[10];
	char *dst=",";
	char *result[16];
	int num=0,i;
	
	GPS_INFO gps;
	
//2.切割字符串	
	printf("11111111111\n");
	result[num]=strtok(str,dst);
	printf("result[%d]->%s\n",num,result[num]);
	printf("22222222222\n");
	while(result[num]!=NULL){
	
		printf("333333333\n");
		num++;
		result[num]=strtok(NULL,dst);
		printf("result[%d]->%s\n",num,result[num]);
	}

//3.给各各结构体成员赋值
	//提取时间
	//year
	strncpy(str1,result[9],2);
	gps.D.year=atoi(str1);
	printf("year->%d\n",gps.D.year);
	
	//month
	strncpy(str1,result[9]+2,2);
	gps.D.month=atoi(str1);
	printf("month->%d\n",gps.D.month);
	
	//day
	strncpy(str1,result[9]+4,2);
	gps.D.day=atoi(str1);
	printf("day->%d\n",gps.D.day);

#if		0
	gps.status=result[2][0];
	gps.latitude=atof(result[3]);	
	strcpy(gps.latitude_c,result[3]);
	printf("status->%c,latitude->%lf,latitude_c->%s\n",gps.status,gps.latitude,gps.latitude_c);
#endif
	return 0;
}
