#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/msg.h>
typedef struct msg{
	long int mtype;
	char mtext[100];
}MSG;
int main()
{
	MSG temp;
	int msgid;
	
	key_t key;
	key=ftok(".",1);
	msgid=msgget(key,IPC_CREAT|0666);
	
	msgrcv(msgid,&temp,sizeof(MSG)-sizeof(long int),2,0);
	printf("recv %s\n",temp.mtext);
	
	msgrcv(msgid,&temp,sizeof(MSG)-sizeof(long int),3,0);
	printf("recv %s\n",temp.mtext);
	
	msgrcv(msgid,&temp,sizeof(MSG)-sizeof(long int),1,0);
	printf("recv %s\n",temp.mtext);
}