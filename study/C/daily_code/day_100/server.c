/*************************************************************************
	> File Name: test2.c
	> Author: 
	> Mail: 
	> Created Time: 2018年03月06日 星期二 17时54分45秒
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
	if(argc>1){
		
		port=atoi(argv[1]);
	}
	
/*创建套接字*/	
	int sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0){
		
		perror("socket");
		exit(-1);
	}
	
/*初始化本地信息*/	
	struct sockaddr_in my_addr;
	bzero(&my_addr,sizeof(my_addr));
	my_addr.sin_family=AF_INET;
	my_addr.sin_port=htons(port);
	my_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	
/*绑定信息*/
	int err_log=bind(sockfd,(struct sockaddr*)&my_addr,sizeof(my_addr));
	if(err_log!=0){
	
		perror("binding");
		close(sockfd);
		exit(-1);
	}
	
/*改变socket的属性*/	
	err_log=listen(sockfd,10);
	if(err_log!=0){
		
		perror("listen");
		close(sockfd);
		exit(-1);
	}
	printf("listen clientport:%d\n",port);
	
	while(1){
	
		struct sockaddr_in client_addr;
		char cli_ip[INET_ADDRSTRLEN]="";
		socklen_t cliaddr_len=sizeof(client_addr);
		
		int connfd;
		connfd=accept(sockfd,(struct sockaddr*)&client_addr,&cliaddr_len);
		if(connfd<0){
			
			perror("accept");
			continue;
		}
		
		inet_ntop(AF_INET,&client_addr.sin_addr,cli_ip,INET_ADDRSTRLEN);
		printf("--------------------\n");
		printf("clent ip %s,port %d\n",cli_ip,ntohs(client_addr.sin_port));
		
		char recv_buf[2048]="";
		while(recv(connfd,recv_buf,sizeof(recv_buf),0)>0){
			
			printf("\nrecv data:\n");
			printf("%s\n",recv_buf);
		}
		
		close(connfd);
		printf("client closed!\n");
	
	}
	
	close(sockfd);
	

    return 0;
}
