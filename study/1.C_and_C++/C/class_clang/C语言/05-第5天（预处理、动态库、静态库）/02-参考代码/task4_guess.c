/*=========================================================================
  �������ƣ�C���Խ׶�-��2��-��ϰ
  ����ļ���Guess.c
  ����������
  ���������
  ά����¼��2010-09-12 v1.0		SunPlusEdu
=========================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <conio.h>
void guess_game(void)
{
	char keylev = 0,key = 0;
	int ges_num = 0,Ges_num[4] = {0},src_num[4] = {0};
	int lev[3] = {10,7,4};
	int level,num = 0;
	int count = 0,i;
	
	srand( (unsigned int )time(NULL) );

	for(i=0;i<4;i++)
		src_num[i]=rand()%10;

	printf("��ѡ���Ѷȼ���:1(10��),2(7��),3(5��)\n");
	
	keylev=getch();
	level=keylev-'1';
	
	printf("Lev.%d\n", level+1);
	printf("��ʼ��Ϸ���������֣�\n");

	while(num<lev[level])
	{
		int i;
		scanf("%d",&ges_num);
		
		Ges_num[0]=(ges_num/1000)%10;
		Ges_num[1]=(ges_num/100)%10;
		Ges_num[2]=(ges_num/10)%10;
		Ges_num[3]=ges_num%10;
		count = 0;
		for(i=0; i<4; i++)
		{
			if(Ges_num[i]>src_num[i])
				printf("��%dλ������ȷ���֡�\n", i+1);
			else if(Ges_num[i]<src_num[i])
				printf("��%dλС����ȷ���֡�\n", i+1);
			else
			{
				printf("��%dλ��ȷ��\n", i+1);
				count++;
			}
		}
		if(count==4)
		{
			printf("�����������ȷ,������˳�:\n");
			getch();
			exit(0);
		}
		num++;
		if(num==lev[level])
			printf("ʧ���ˣ�����\n");
		else
			printf("�ٴ���������:\n");	
	}
}
void rule(void)
{
	char key;

	printf("�����֣�ϵͳ���������һ����λ���֣��ڲ�ͬ�Ѷ�\n");
	printf("��������Ӧ�Ĵ���ȥ�²�������֡�ϵͳ�������ʾ\n");
	printf("��Ϣ���ڹ涨�����ڲ¶Լ���ʤ��\n");
	
	printf("��1  ������Ϸ\n��0  �˳�\n");
	
	key=getch();

	if(key=='0')
		exit(0);
	else if(key=='1')
		guess_game();
}

int main(void)
{
	char key = 0;
	while(1)
	{
		printf("��1  ������Ϸ\n��2  �鿴��Ϸ����\n��0  �˳�\n");
		key=getch();
		if(key == '0')
			exit(0);		//�˳�����
		else if(key == '1')
			guess_game();	//������Ϸ
		else if(key == '2')
			rule();			//�鿴��Ϸ����
	}
	return 0;
}
