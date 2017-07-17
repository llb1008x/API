
/*=========================================================================
  工程名称：	练习  
  组成文件：	main.c LRC.c LRC.h pos.c 
  功能描述： 	解析并同步显示歌词信息
  程序分析：	
  维护记录：	2010-09-12 v1.1		add by dxh
=========================================================================*/

#include <stdio.h>
//#include <conio.h>
#include "./LRC/LRC.h"
#include <windows.h> 

extern void whereXY(int *x,int *y);
extern void GoToXY(int x,int y);

int x=20,y=6;


LRC_INFO *play_song(unsigned int num,LRC_INFO *head)
{
	static int count=0,i=0;//count记录显示到了那一句歌词
	static int flag=0;
	static LRC_INFO *Temp;

	if(flag==0)
	{
		Temp=head;
		flag=1;
	}
	if(count<6)
	{
		if( num == ( Temp->Time / 100 ) )	//与时间标签对比，判断是否该显示歌词
		{
			//printf("*%d*",count);
			system("color 07");
			SetText_Color(0x02);
			printf("\n\t\t%s",Temp->LrcText);
			whereXY(&x,&y);					//保存歌词打印后位置
			Temp = Temp->next;				//显示相应歌词，调整歌词链表到下一个节点
			count++;
		}	
	}
	else
	{
	
		if( num == ( Temp->Time / 100 ) )	
		{
			SetText_Color(0x07);
			system("cls");	//清屏
			Message_Song_Play();
			//printf("*%d*",count);
			for(i=0;i<count-6;i++) //count<=6时，head指向头不变，当count>6时，
			{					   	//将head移到当前节点的前6个节点处		
				head = head->next;				
			}
			GoToXY(20,6);						 
			for(i=0;i<6;i++)	//显示当前句的前6句
			{
				printf("\n\t\t%s",head->LrcText);
				head = head->next;			
			}
			SetText_Color(0x02);
			printf("\n\t\t%s",Temp->LrcText);
			Temp = Temp->next;				
			//whereXY(&x,&y);				
			count++;
		}
	}
	return Temp;
}	

int main()
{
	FILE *fd;
	int size,rsize;
	char *pbuffer;	
	LRC_INFO * head;
	LRC_INFO * Temp;
	unsigned int num = 0;	
	int Song_Minute=0,Song_Second=0;			//与时间轴相关变量

#if 0	
	system("color 0 0");//非法操作，提示正确操作说明
	system("color 47");//背景红，前景白
	SetText_Color(0x47);//文字背景红，前景白
	system("pause");
	system("cls");	//清屏
#endif 	
//if( (fd = fopen("D:\\study_test\\clanguage\\lrc_display_demo\\LRC\\简单爱.lrc","rb")) == NULL )
	if( (fd = fopen("./LRC/简单爱.lrc","rb")) == NULL )
	//if( (fd = fopen("简单爱.lrc","rb")) == NULL )
	{
		printf("No Lrc File!\n");				//打开歌词文件
	}	
	else
	{ 
		fseek(fd,0,2);
		size = ftell(fd);
		rewind(fd);
		
		pbuffer = (char *)malloc(size+1);
		rsize = fread(pbuffer,1,size,fd);		//将全部歌词读到缓冲区
		fclose(fd);		
		*(pbuffer+rsize)='\0';
		//printf("size=%d\n\n%s\n",size,pbuffer);	//打印所有歌词内容到屏幕上
		//return 0;
	}
	
	Message_Song(pbuffer);

	Message_Song_Play();
	head = GetLrcInfo(pbuffer, size);			//调用函数排序
	Temp=head;
//	print(head);			//将排好序的链表打印输出
//	return 0;
	ShellExecute( NULL, NULL, "TTPlayer.exe",".\\LRC\\简单爱.mp3",NULL,1);	//执行千千静听
	//ShellExecute( NULL, NULL, "TTPlayer.exe","简单爱.mp3",NULL,1);	//执行千千静听
	Sleep(200);
	while(Temp!=NULL)
	{
		GoToXY(20,5);						 //在指定位置打印时间
		SetText_Color(0x07);
		printf("the time is:%d:%d\n",Song_Minute,Song_Second);
		GoToXY(x,y);						//返回歌词显示位置
		Sleep(1000);
		num++;								//时间计数标志(秒)
		if(++Song_Second == 60)				//打印显示时间轴
		{
			Song_Second = 0;
			if(++Song_Minute == 60)
			{
				Song_Minute = 0;
			}
		}
		Temp=play_song(num,head);
	}
}