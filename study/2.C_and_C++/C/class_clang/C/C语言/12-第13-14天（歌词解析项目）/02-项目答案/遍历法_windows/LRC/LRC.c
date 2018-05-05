//====================================================================================
//文 件 名：LRC.c
//功能描述: LRC解析程序
//维护记录:
//			2009.02.20		V1.0
//====================================================================================

#include <string.h>
#include "LRC.h"
#include <stdio.h>
#include <stdlib.h>

#define isnumeric(a) (  (  (a >= '0') && (a <= '9') ) ? 1 : 0 )

char message_song[4][100]={0};
//=============================================================
//语法格式： 	Message_Song(char *Lrc_Buffer);
//实现功能：	解析歌词，将歌曲的作者、歌曲名、专辑、歌词制作信息取出
//参数：		
//返回值：		无
//=============================================================

extern char message_song[4][100];

void Message_Song(char *Lrc_Buffer)
{
	int offset=0;
	int temp,i=0;

	char *message[4]={"[ti","[ar","[al","[by"};
	char *name[4]={"歌名:","歌手:","专辑:","制作:"};

	for(i=0;i<4;i++)
	{
		while( Lrc_Buffer[offset] != '[' )
			offset++;
	
		if (strncmp(Lrc_Buffer+offset,message[i],3)==0)
		{
			temp = offset+4;  					//跳过 ti: 三个字母
			while( Lrc_Buffer[offset] !=']' )
				offset++;     					//找到歌曲名的结尾
			Lrc_Buffer[offset]='\0';
			strcpy(message_song[i],&Lrc_Buffer[temp]);
		}
	}
}

void Message_Song_Play()
{
	char *name[4]={"歌名:","歌手:","专辑:","制作:"};
	int i;
	for(i=0;i<4;i++)
	{
		printf("%s\t\t%s\n",name[i],message_song[i]);
	}
}

//=============================================================
//语法格式： 	GetLrcInfo(char *LrcFile, u32_t FileSize);
//实现功能：	解析歌词，将每一句歌词按时间、内容解析出来，并插入到链表中
//参数：		LrcFile：存放全部歌词的buffer
//				FileSize：文件大小
//返回值：		排好序的链表首地址
//=============================================================
 LRC_INFO *GetLrcInfo(char *LrcFile, u32_t FileSize)
{
	s32_t OffsetTime = 0;
	u32_t CurOffset = 0;
	u32_t CurTime[10] = {0}; //保存同一句歌词重复出现的时间点，最多重复10次
	u32_t TempOffset;	//时间补偿值
	int num=0,i=0; //num表示同一句歌词重复出现的次数

	int Flag = 0;
	LRC_INFO *head= NULL;			//待插入链表

	LRC_INFO *lrc_link;				//歌词节点

	while(CurOffset < FileSize)						//循环解析出所有的歌词
	{
		if(LrcFile[CurOffset] == '[')		
		{
			CurOffset++;
			
			if(strncmp(&LrcFile[CurOffset], "offset:", 7) == 0)	//检测到时间补偿标签
			{
				CurOffset += 7;
				if(LrcFile[CurOffset] == '-')
				{
					Flag = -1;
					CurOffset++;
				}
				else
					Flag = 1;
				while(LrcFile[CurOffset] != ']')//组合正整数
				{
					OffsetTime *= 10;
					OffsetTime += LrcFile[CurOffset++] - '0';
				}
				OffsetTime = OffsetTime / 10 * Flag;	//将时间补偿值毫秒变成10ms为单位
			}
			
			if((LrcFile[CurOffset]) >= '0' && (LrcFile[CurOffset]) <= '9'  ) //检测到歌词时间标签
			{
				char *lrc;

				while(LrcFile[CurOffset] >='0' && LrcFile[CurOffset] <='9')//取出这句歌词所有时间标签
				{
					CurTime[num++] = ( (  (LrcFile[CurOffset] - '0') * 10 + (LrcFile[CurOffset + 1] - '0')) * 60 + \
						((LrcFile[CurOffset + 3] - '0') * 10 + LrcFile[CurOffset + 4] - '0')) * 100 + (LrcFile[CurOffset + 6] - '0') * 10 + LrcFile[CurOffset + 7] - '0';

					CurOffset=CurOffset+10;		//解析一句歌词在不同时间出现的情况
				}

				lrc = &LrcFile[CurOffset-1];//得到这句歌词首地址
				while( LrcFile[CurOffset] !='\n')
					CurOffset++;
				LrcFile[CurOffset] =  '\0' ;		//修改buffer将'\n'换成'\0'，构成多个字符串
				
				for(i=0;i<num;i++)//跟据这句歌词得复出现时间插入到链表各节点中
				{
					lrc_link =  ( LRC_INFO *)malloc(sizeof(LRC_INFO));  
					lrc_link->Time = CurTime[i]+OffsetTime;
					strcpy (lrc_link->LrcText,lrc);			//申请节点，存放解析出歌词
					//将解析出的时间、歌词内容插入到单向链表并进行排序	
					head = InsertList(head,lrc_link); 
				}
				num=0;				
			}
		}
		CurOffset++;
	} 

	return head;														
}

//=============================================================
//语法格式： 	LRC_INFO *InsertList( LRC_INFO * head, LRC_INFO* pi)
//实现功能：	将解析出的歌词按顺序插入链表
//参数：		head: 待排序链表
//				pi:待插入节点
//返回值：		排好序的链表首地址
//=============================================================
LRC_INFO *InsertList( LRC_INFO * head, LRC_INFO* pi)
{
	 LRC_INFO *pf,*pb;
	 pb=head;
	
	 if(head==NULL) 					//原表为空表，则直接加入
	 {
		 head=pi;
		 pi->next=NULL;
	 }
	 else							//原表非空
	 {
		 while( (pi->Time > pb->Time) && (pb->next!=NULL) )	//从头开始找到比插入时间表小的节点
		 {
			 pf=pb;						//pf指向当前节点的前一个节点，节点后移
			 pb=pb->next;
		 }							
		 if(pi->Time <= pb->Time)		//找到所要插入节点位置
		 {
			 if(head==pb)//如果第2个节点时间小于头节点，则与头节点交换位置
			 {
				 head=pi;			//在第一结点之前插入
				 pi->next = pb;	//add by xiuhai
			 }
			 else 
			 {	
				 pf->next=pi;		//在中间位置插入
				 pi->next=pb;
			 }	
		 }
		 else
		 {
			 pb->next=pi;
			 pi->next=NULL;
		 } 							//在表末插入
	 }
	 return head;
}
//=============================================================
//语法格式： 	void print( LRC_INFO * head)
//实现功能：	打印指定链表各节点所有内容
//参数：		head: 待排序链表首地址
//返回值：		无
//=============================================================
void print( LRC_INFO * head)
{
	LRC_INFO *p;
	p = head;
	do
	{
		printf("%d,%s\n",p->Time,p->LrcText);
		p = p->next;
	}
	while(p!=NULL);
}

