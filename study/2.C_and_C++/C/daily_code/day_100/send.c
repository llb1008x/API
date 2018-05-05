/*************************************************************************
	> File Name: test2.c
	> Author: 
	> Mail: 
	> Created Time: 2018年03月06日 星期二 15时42分55秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>


int main(int argc,char *argv[])
{
	unsigned short port=8080;
	char *server_ip="10.8.8.69";
	
/*获取port, addr*/	
	if(argc>1){
		
		server_ip=argv[1];
	}
	
	if(argc>2){
		
		port=atoi(argv[2]);
	}

/*创建套接字*/	
	int sockfd;
	sockfd=socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd<0){
		
		perror("socket");
		exit(-1);
	}
	
/*server相关的信息*/	
	struct sockaddr_in dest_addr;
	bzero(&dest_addr,sizeof(dest_addr));
	dest_addr.sin_family=AF_INET;
	dest_addr.sin_port=htons(port);
	inet_pton(AF_INET,server_ip,&dest_addr.sin_addr);
	
	printf("send data to udp server:%s:%d\n",server_ip,port);
	
/*发送数据*/
	while(1){
		
		char send_buf[512]="";
		fgets(send_buf,sizeof(send_buf),stdin);
		sendto(sockfd,send_buf,strlen(send_buf),0,(struct sockaddr*)&dest_addr,sizeof(dest_addr));
		
	}			
	
	close(sockfd);
	
	return 0;
}
