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
	
	temp.mtype=1;
	strcpy(temp.mtext,"hello world  1 ");
	
	msgsnd(msgid,&temp,sizeof(MSG)-sizeof(long int),0);
	
	temp.mtype=2;
	strcpy(temp.mtext,"hello world  2");
	msgsnd(msgid,&temp,sizeof(MSG)-sizeof(long int),0);
	
	temp.mtype=3;
	strcpy(temp.mtext,"hello world 3");
	msgsnd(msgid,&temp,sizeof(MSG)-sizeof(long int),0);
}