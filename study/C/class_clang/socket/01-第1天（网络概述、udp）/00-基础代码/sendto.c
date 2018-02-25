/* ************************************************************************
 *       Filename:  sendto.c
 *    Description:  
 *        Version:  1.0
 *        Created:  2016年06月28日 17时25分38秒
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  cr@613.com
 *        Company:  
 * ************************************************************************/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
int main(int argc, char *argv[])
{
	int sockfd=0;
	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd<0)
	{
		perror("socket");
		exit(-1);
	}

	struct sockaddr_in my_addr;
	bzero(&my_addr,sizeof(my_addr));
	my_addr.sin_family=AF_INET;
	my_addr.sin_port=htons(8000);
	my_addr.sin_addr.s_addr=htonl(INADDR_ANY);
//	inet_pton(AF_INET,"10.220.4.20",&my_addr.sin_addr);
	
	if(bind(sockfd,(struct sockaddr *)&my_addr,sizeof(my_addr))!=0)
	{
		perror("bind");
		close(sockfd);
		exit(-1);
	}


	struct sockaddr_in dest_addr;
	bzero(&dest_addr,sizeof(dest_addr));
	dest_addr.sin_family=AF_INET;
	dest_addr.sin_port=htons(8080);
	inet_pton(AF_INET,"10.220.4.100",&dest_addr.sin_addr);

	char send_buf[100]="hello_world";
	sendto(sockfd,send_buf,strlen(send_buf),0,
			(struct sockaddr *)&dest_addr,sizeof(dest_addr));
	
	char recv_buf[100]="";
	struct sockaddr_in s_addr;
	bzero(&s_addr,sizeof(s_addr));
	int len=sizeof(s_addr);
	recvfrom(sockfd,recv_buf,sizeof(recv_buf),0,
			(struct sockaddr *)&s_addr,&len);


	printf("recv_buf------------%s\n",recv_buf);
	printf("len=====%d\n",len);
	if(s_addr.sin_family==AF_INET)
	{
		
		printf("AF_INET\n");
	}

	close(sockfd);
	return 0;
}


