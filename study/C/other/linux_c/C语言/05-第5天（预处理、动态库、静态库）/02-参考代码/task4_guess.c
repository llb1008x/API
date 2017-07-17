/*=========================================================================
  工程名称：C语言阶段-第2天-练习
  组成文件：Guess.c
  功能描述：
  程序分析：
  维护记录：2010-09-12 v1.0		SunPlusEdu
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

	printf("请选择难度级别:1(10次),2(7次),3(5次)\n");
	
	keylev=getch();
	level=keylev-'1';
	
	printf("Lev.%d\n", level+1);
	printf("开始游戏。输入数字：\n");

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
				printf("第%d位大于正确数字。\n", i+1);
			else if(Ges_num[i]<src_num[i])
				printf("第%d位小于正确数字。\n", i+1);
			else
			{
				printf("第%d位正确。\n", i+1);
				count++;
			}
		}
		if(count==4)
		{
			printf("你输入的数正确,任意键退出:\n");
			getch();
			exit(0);
		}
		num++;
		if(num==lev[level])
			printf("失败了！！！\n");
		else
			printf("再次输入数字:\n");	
	}
}
void rule(void)
{
	char key;

	printf("猜数字：系统会随机产生一个四位数字，在不同难度\n");
	printf("下你有相应的次数去猜测这个数字。系统会给你提示\n");
	printf("信息，在规定次数内猜对即获胜。\n");
	
	printf("按1  进入游戏\n按0  退出\n");
	
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
		printf("按1  进入游戏\n按2  查看游戏规则\n按0  退出\n");
		key=getch();
		if(key == '0')
			exit(0);		//退出程序
		else if(key == '1')
			guess_game();	//进入游戏
		else if(key == '2')
			rule();			//查看游戏规则
	}
	return 0;
}
