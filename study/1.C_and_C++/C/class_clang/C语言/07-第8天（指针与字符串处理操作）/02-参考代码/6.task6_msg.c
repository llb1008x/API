#include <stdio.h>
int main()
{
	char msg_src[]={"+CMGR:REC UNREAD,+8613466630259,98/10/01,18:22:11+00,ABCdefGHI"};
	char *buf[6];
	char *msg[4] = {"电话：","日期：","时间：","内容："};
	int i = 0;
	
	buf[i] = strtok(msg_src,",");
	while(buf[i] != NULL){
		buf[++i] = strtok(NULL,",");
	}
	buf[1] = buf[1]+3;
	*(buf[3]+8) = '\0';
	for(i--;i>0;i--){
		printf("%s%s\n",msg[i-1],buf[i]);
	}
	return 0;
}

