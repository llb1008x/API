//====================================================================================
//�� �� ����LRC.c
//��������: LRC��������
//ά����¼:
//			2009.02.20		V1.0
//====================================================================================

#include <string.h>
#include "LRC.h"
#include <stdio.h>
#include <stdlib.h>

#define isnumeric(a) (  (  (a >= '0') && (a <= '9') ) ? 1 : 0 )

char message_song[4][100]={0};
//=============================================================
//�﷨��ʽ�� 	Message_Song(char *Lrc_Buffer);
//ʵ�ֹ��ܣ�	������ʣ������������ߡ���������ר�������������Ϣȡ��
//������		
//����ֵ��		��
//=============================================================

extern char message_song[4][100];

void Message_Song(char *Lrc_Buffer)
{
	int offset=0;
	int temp,i=0;

	char *message[4]={"[ti","[ar","[al","[by"};
	char *name[4]={"����:","����:","ר��:","����:"};

	for(i=0;i<4;i++)
	{
		while( Lrc_Buffer[offset] != '[' )
			offset++;
	
		if (strncmp(Lrc_Buffer+offset,message[i],3)==0)
		{
			temp = offset+4;  					//���� ti: ������ĸ
			while( Lrc_Buffer[offset] !=']' )
				offset++;     					//�ҵ��������Ľ�β
			Lrc_Buffer[offset]='\0';
			strcpy(message_song[i],&Lrc_Buffer[temp]);
		}
	}
}

void Message_Song_Play()
{
	char *name[4]={"����:","����:","ר��:","����:"};
	int i;
	for(i=0;i<4;i++)
	{
		printf("%s\t\t%s\n",name[i],message_song[i]);
	}
}

//=============================================================
//�﷨��ʽ�� 	GetLrcInfo(char *LrcFile, u32_t FileSize);
//ʵ�ֹ��ܣ�	������ʣ���ÿһ���ʰ�ʱ�䡢���ݽ��������������뵽������
//������		LrcFile�����ȫ����ʵ�buffer
//				FileSize���ļ���С
//����ֵ��		�ź���������׵�ַ
//=============================================================
 LRC_INFO *GetLrcInfo(char *LrcFile, u32_t FileSize)
{
	s32_t OffsetTime = 0;
	u32_t CurOffset = 0;
	u32_t CurTime[10] = {0}; //����ͬһ�����ظ����ֵ�ʱ��㣬����ظ�10��
	u32_t TempOffset;	//ʱ�䲹��ֵ
	int num=0,i=0; //num��ʾͬһ�����ظ����ֵĴ���

	int Flag = 0;
	LRC_INFO *head= NULL;			//����������

	LRC_INFO *lrc_link;				//��ʽڵ�

	while(CurOffset < FileSize)						//ѭ�����������еĸ��
	{
		if(LrcFile[CurOffset] == '[')		
		{
			CurOffset++;
			
			if(strncmp(&LrcFile[CurOffset], "offset:", 7) == 0)	//��⵽ʱ�䲹����ǩ
			{
				CurOffset += 7;
				if(LrcFile[CurOffset] == '-')
				{
					Flag = -1;
					CurOffset++;
				}
				else
					Flag = 1;
				while(LrcFile[CurOffset] != ']')//���������
				{
					OffsetTime *= 10;
					OffsetTime += LrcFile[CurOffset++] - '0';
				}
				OffsetTime = OffsetTime / 10 * Flag;	//��ʱ�䲹��ֵ������10msΪ��λ
			}
			
			if((LrcFile[CurOffset]) >= '0' && (LrcFile[CurOffset]) <= '9'  ) //��⵽���ʱ���ǩ
			{
				char *lrc;

				while(LrcFile[CurOffset] >='0' && LrcFile[CurOffset] <='9')//ȡ�����������ʱ���ǩ
				{
					CurTime[num++] = ( (  (LrcFile[CurOffset] - '0') * 10 + (LrcFile[CurOffset + 1] - '0')) * 60 + \
						((LrcFile[CurOffset + 3] - '0') * 10 + LrcFile[CurOffset + 4] - '0')) * 100 + (LrcFile[CurOffset + 6] - '0') * 10 + LrcFile[CurOffset + 7] - '0';

					CurOffset=CurOffset+10;		//����һ�����ڲ�ͬʱ����ֵ����
				}

				lrc = &LrcFile[CurOffset-1];//�õ�������׵�ַ
				while( LrcFile[CurOffset] !='\n')
					CurOffset++;
				LrcFile[CurOffset] =  '\0' ;		//�޸�buffer��'\n'����'\0'�����ɶ���ַ���
				
				for(i=0;i<num;i++)//��������ʵø�����ʱ����뵽������ڵ���
				{
					lrc_link =  ( LRC_INFO *)malloc(sizeof(LRC_INFO));  
					lrc_link->Time = CurTime[i]+OffsetTime;
					strcpy (lrc_link->LrcText,lrc);			//����ڵ㣬��Ž��������
					//����������ʱ�䡢������ݲ��뵽����������������	
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
//�﷨��ʽ�� 	LRC_INFO *InsertList( LRC_INFO * head, LRC_INFO* pi)
//ʵ�ֹ��ܣ�	���������ĸ�ʰ�˳���������
//������		head: ����������
//				pi:������ڵ�
//����ֵ��		�ź���������׵�ַ
//=============================================================
LRC_INFO *InsertList( LRC_INFO * head, LRC_INFO* pi)
{
	 LRC_INFO *pf,*pb;
	 pb=head;
	
	 if(head==NULL) 					//ԭ��Ϊ�ձ���ֱ�Ӽ���
	 {
		 head=pi;
		 pi->next=NULL;
	 }
	 else							//ԭ��ǿ�
	 {
		 while( (pi->Time > pb->Time) && (pb->next!=NULL) )	//��ͷ��ʼ�ҵ��Ȳ���ʱ���С�Ľڵ�
		 {
			 pf=pb;						//pfָ��ǰ�ڵ��ǰһ���ڵ㣬�ڵ����
			 pb=pb->next;
		 }							
		 if(pi->Time <= pb->Time)		//�ҵ���Ҫ����ڵ�λ��
		 {
			 if(head==pb)//�����2���ڵ�ʱ��С��ͷ�ڵ㣬����ͷ�ڵ㽻��λ��
			 {
				 head=pi;			//�ڵ�һ���֮ǰ����
				 pi->next = pb;	//add by xiuhai
			 }
			 else 
			 {	
				 pf->next=pi;		//���м�λ�ò���
				 pi->next=pb;
			 }	
		 }
		 else
		 {
			 pb->next=pi;
			 pi->next=NULL;
		 } 							//�ڱ�ĩ����
	 }
	 return head;
}
//=============================================================
//�﷨��ʽ�� 	void print( LRC_INFO * head)
//ʵ�ֹ��ܣ�	��ӡָ��������ڵ���������
//������		head: �����������׵�ַ
//����ֵ��		��
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

