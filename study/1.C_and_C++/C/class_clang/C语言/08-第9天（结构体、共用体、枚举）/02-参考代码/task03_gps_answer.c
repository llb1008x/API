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
	char time_c[10];//ACSII��ʽ��ʱ��
}date_time;

typedef struct{
	date_time D;						//ʱ��
	char status;  					//����״̬
	double latitude;   		//γ��
	char   latitude_c[11];	//ASCII��ʽ��γ��
	double longitude; 		  //����
	char   longitude_c[11];//ASCII��ʽ�ľ���
	char NS;          			//�ϱ���
	char EW;           		//����
	double speed;      		//�ٶ�
	char   speed_c[10];		//ASCII��ʽ���ٶ�
	double high;       		//�߶�
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

	p[i]=strtok(str,",");				//��","�Ŷ��ַ��������и�
	while(p[i])
		p[++i]=strtok(NULL,",");

	g.D.year=atoi(*(p+9)+4);			//����9�ֶε�year��ȡ����

	strncpy(str1,*(p+9)+2,2);			//��ȡ��
	str1[2]='\0';
	g.D.month=atoi(str1);

	strncpy(str1,*(p+9),2);				//��ȡ��
	str1[2]='\0';
	g.D.day=atoi(str1);

	strncpy(str1,*(p+1),2);				//��ȡʱ
	str1[2]='\0';
//	strcpy(g.D.time_c,str1);			
	g.D.hour=atoi(str1)+8;
	*g.D.time_c=atoi(str1)+8;



	strncpy(str1,*(p+1)+2,2);			//��ȡ��
	str1[2]='\0';
//	strcat(g.D.time_c,str1);
	g.D.minute=atoi(str1);

	strncpy(str1,*(p+1)+4,6);			//��ȡ��
	str1[6]='\0';
//	strcat(g.D.time_c,str1);
	g.D.second=atoi(str1);

	g.latitude=atof(*(p+3));			//��ȡ���ȱ���Ϊ�ַ�����double
	strcpy(g.latitude_c,*(p+3));
	g.NS=*p[4];

	g.longitude=atof(*(p+5));			//��ȡά�ȱ���Ϊ�ַ�����double
	strcpy(g.longitude_c,*(p+5));
	g.EW=*p[6];

	printf("gps_parse result:\n\n");		//��ӡ����Ϣ
										//��ӡʱ��
	printf("ʱ��:%02d:%02d:%02d\n",g.D.hour,g.D.minute,g.D.second);

	printf("����:%02d��%02d��%02d��\n",g.D.year,g.D.month,g.D.day);

	strncpy(str1,g.latitude_c,2);		//��ӡγ��
	*(str1+2)='\0';
	printf("γ��:%cγ%s��%s��\n",g.NS,str1,g.latitude_c+2);

	strncpy(str1,g.longitude_c,3);		//��ӡ����
	*(str1+3)='\0';
	printf("����:%c��%s��%s��\n",g.EW,str1,g.longitude_c+3);

}

/*
int gps_deal(struct gps gps_mes,char *message)
*/