#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/ipc.h>
#include <string.h> 
#include <sys/types.h>
//定义三个发送人。

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


typedef struct msg_my 	//消息结构体
{ 
	long mtype; 		//用来存储消息的类型
	char name[20];		//用来存发送者的名字
	char mtext[100]; 	//用来存储真正要发送的消息的内容
}MSG; 

int main(int argc, char **argv) 
{ 
	char buff[120]="";
	int msgid; 
	MSG msg;
	
	//key_t key;  
	//key=ftok(".", 0xFF); 
	if((msgid=msgget((key_t)12345, IPC_CREAT|0666))<0)//创建消息队列。 
	{ 
		perror("msgid"); 
		exit(1); 
	}
	if(fork()==0)
	{//检测键盘，组织数据，发送消息到消息队列
		while(1)
		{
			printf("\r%s:",writer);//打印发送人提示
			fflush(stdout);
			fgets(buff,sizeof(buff),stdin); //从键盘获取输入
			buff[strlen(buff)-1]='\0'; 
			if(buff[0] == 'L' || buff[0] == 'P'|| buff[0] == 'B')
			{
				msg.mtype=(long )buff[0]; //消息类型。
				strcpy(msg.name, writer);
				strcpy(msg.mtext, buff+2); 
				msgsnd(msgid, &msg,sizeof(msg)-sizeof(long), 0);  // 添加消息。
			}
		}
	}
	else
	{//从消息队列中，按照“我”消息类型接收消息
		while(1)
		{
			//只接受自己名字首字母大写类型的消息。  
			msgrcv(msgid, &msg,sizeof(msg)-sizeof(long), (long int)(M), 0);
			printf("\rmessage from %s: %s\n",msg.name,msg.mtext);
			printf("\r%s:",writer);
			fflush(stdout);
		}
	}
}

