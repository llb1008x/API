/*************************************************************************
	> File Name: recv.c
	> Author: 
	> Mail: 
	> Created Time: 2018年03月06日 星期二 17时32分32秒
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
/*创建sockfd*/	
	int sockfd;
	sockfd=socket(AF_INET,SOCK_DGRAM,0);
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
	
/*绑定本地信息*/	
	printf("binding server to port %d\n",port);
	int err_log;
	err_log=bind(sockfd,(struct sockaddr*)&my_addr,sizeof(my_addr));
	if(err_log!=0){
		
		perror("bind");
		close(sockfd);
		exit(-1);
	}	

/*接受数据*/
	printf("receive data ....\n");
	while(1){
		
		int recv_len;
		char recv_buf[512]="";
		struct sockaddr_in client_addr;
		char cli_ip[INET_ADDRSTRLEN]="";
		socklen_t cliaddr_len=sizeof(client_addr);
		
		recv_len=recvfrom(sockfd,recv_buf,sizeof(recv_buf),0,(struct sockaddr *)&client_addr,&cliaddr_len);
		inet_ntop(AF_INET,&client_addr.sin_addr,cli_ip,INET_ADDRSTRLEN);
		printf("\n ip:%s,port:%d\n",cli_ip,ntohs(client_addr.sin_port));
		printf("data(%d):%s\n",recv_len,recv_buf);
	}	
	
	close(sockfd);
	
	
    return 0;
}
