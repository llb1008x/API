
/*=========================================================================
  �������ƣ�	��ϰ  
  ����ļ���	main.c LRC.c LRC.h pos.c 
  ���������� 	������ͬ����ʾ�����Ϣ
  ���������	
  ά����¼��	2010-09-12 v1.1		add by dxh
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
	static int count=0,i=0;//count��¼��ʾ������һ����
	static int flag=0;
	static LRC_INFO *Temp;

	if(flag==0)
	{
		Temp=head;
		flag=1;
	}
	if(count<6)
	{
		if( num == ( Temp->Time / 100 ) )	//��ʱ���ǩ�Աȣ��ж��Ƿ����ʾ���
		{
			//printf("*%d*",count);
			system("color 07");
			SetText_Color(0x02);
			printf("\n\t\t%s",Temp->LrcText);
			whereXY(&x,&y);					//�����ʴ�ӡ��λ��
			Temp = Temp->next;				//��ʾ��Ӧ��ʣ��������������һ���ڵ�
			count++;
		}	
	}
	else
	{
	
		if( num == ( Temp->Time / 100 ) )	
		{
			SetText_Color(0x07);
			system("cls");	//����
			Message_Song_Play();
			//printf("*%d*",count);
			for(i=0;i<count-6;i++) //count<=6ʱ��headָ��ͷ���䣬��count>6ʱ��
			{					   	//��head�Ƶ���ǰ�ڵ��ǰ6���ڵ㴦		
				head = head->next;				
			}
			GoToXY(20,6);						 
			for(i=0;i<6;i++)	//��ʾ��ǰ���ǰ6��
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
	int Song_Minute=0,Song_Second=0;			//��ʱ������ر���

#if 0	
	system("color 0 0");//�Ƿ���������ʾ��ȷ����˵��
	system("color 47");//�����죬ǰ����
	SetText_Color(0x47);//���ֱ����죬ǰ����
	system("pause");
	system("cls");	//����
#endif 	
//if( (fd = fopen("D:\\study_test\\clanguage\\lrc_display_demo\\LRC\\�򵥰�.lrc","rb")) == NULL )
	if( (fd = fopen("./LRC/�򵥰�.lrc","rb")) == NULL )
	//if( (fd = fopen("�򵥰�.lrc","rb")) == NULL )
	{
		printf("No Lrc File!\n");				//�򿪸���ļ�
	}	
	else
	{ 
		fseek(fd,0,2);
		size = ftell(fd);
		rewind(fd);
		
		pbuffer = (char *)malloc(size+1);
		rsize = fread(pbuffer,1,size,fd);		//��ȫ����ʶ���������
		fclose(fd);		
		*(pbuffer+rsize)='\0';
		//printf("size=%d\n\n%s\n",size,pbuffer);	//��ӡ���и�����ݵ���Ļ��
		//return 0;
	}
	
	Message_Song(pbuffer);

	Message_Song_Play();
	head = GetLrcInfo(pbuffer, size);			//���ú�������
	Temp=head;
//	print(head);			//���ź���������ӡ���
//	return 0;
	ShellExecute( NULL, NULL, "TTPlayer.exe",".\\LRC\\�򵥰�.mp3",NULL,1);	//ִ��ǧǧ����
	//ShellExecute( NULL, NULL, "TTPlayer.exe","�򵥰�.mp3",NULL,1);	//ִ��ǧǧ����
	Sleep(200);
	while(Temp!=NULL)
	{
		GoToXY(20,5);						 //��ָ��λ�ô�ӡʱ��
		SetText_Color(0x07);
		printf("the time is:%d:%d\n",Song_Minute,Song_Second);
		GoToXY(x,y);						//���ظ����ʾλ��
		Sleep(1000);
		num++;								//ʱ�������־(��)
		if(++Song_Second == 60)				//��ӡ��ʾʱ����
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