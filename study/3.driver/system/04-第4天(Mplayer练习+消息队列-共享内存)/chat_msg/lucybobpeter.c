#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/ipc.h>
#include <string.h> 
#include <sys/types.h>
//�������������ˡ�

#ifdef L
#define M 'L'
char writer[]="Lucy";
#endif

#ifdef B
#define M 'B'
char writer[]="Bob";
#endif

#ifdef P
#define M 'P'
char writer[]="Peter"; 
#endif


typedef struct msg_my 	//��Ϣ�ṹ��
{ 
	long mtype; 		//�����洢��Ϣ������
	char name[20];		//�����淢���ߵ�����
	char mtext[100]; 	//�����洢����Ҫ���͵���Ϣ������
}MSG; 

int main(int argc, char **argv) 
{ 
	char buff[120]="";
	int msgid; 
	MSG msg;
	
	//key_t key;  
	//key=ftok(".", 0xFF); 
	if((msgid=msgget((key_t)12345, IPC_CREAT|0666))<0)//������Ϣ���С� 
	{ 
		perror("msgid"); 
		exit(1); 
	}
	if(fork()==0)
	{//�����̣���֯���ݣ�������Ϣ����Ϣ����
		while(1)
		{
			printf("\r%s:",writer);//��ӡ��������ʾ
			fflush(stdout);
			fgets(buff,sizeof(buff),stdin); //�Ӽ��̻�ȡ����
			buff[strlen(buff)-1]='\0'; 
			if(buff[0] == 'L' || buff[0] == 'P'|| buff[0] == 'B')
			{
				msg.mtype=(long )buff[0]; //��Ϣ���͡�
				strcpy(msg.name, writer);
				strcpy(msg.mtext, buff+2); 
				msgsnd(msgid, &msg,sizeof(msg)-sizeof(long), 0);  // �����Ϣ��
			}
		}
	}
	else
	{//����Ϣ�����У����ա��ҡ���Ϣ���ͽ�����Ϣ
		while(1)
		{
			//ֻ�����Լ���������ĸ��д���͵���Ϣ��  
			msgrcv(msgid, &msg,sizeof(msg)-sizeof(long), (long int)(M), 0);
			printf("\rmessage from %s: %s\n",msg.name,msg.mtext);
			printf("\r%s:",writer);
			fflush(stdout);
		}
	}
}

