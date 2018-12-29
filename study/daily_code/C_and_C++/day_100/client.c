/*************************************************************************
	> File Name: client.c
	> Author: 
	> Mail: 
	> Created Time: 2018年03月06日 星期二 19时35分55秒
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

/*配置默认信息*/
	unsigned short port=8080;
	char *server_ip="10.8.8.69";
	
	if(argc>1){
		
		server_ip=argv[1];
	}
	
	if(argc>2){
		
		port=atoi(argv[2]);
	}
	
/*创建套接字*/
	int sockfd=0;
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0){
		
		perror("socket");
		exit(-1);
	}	
	
/*配置服务器信息*/
	struct sockaddr_in server_addr;
	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_port=htons(port);
	inet_pton(AF_INET,server_ip,&server_addr.sin_addr);
	
/*连接服务器*/	
	int err_log=connect(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr));
	if(err_log!=0){
		
		perror("connect");
		close(sockfd);
		exit(-1);
	}	
	
	char send_buf[512]="";
	char recv_buf[512]="";
	printf("send data to %s:%d\n",server_ip,port);
	
/*发送信息*/
	printf("send:");
	fgets(send_buf,sizeof(send_buf),stdin);
	send_buf[strlen(send_buf)-1]=0;
	send(sockfd,recv_buf,strlen(send_buf),0);
	
/*接受信息*/
	recv(sockfd,recv_buf,sizeof(recv_buf),0);
	printf("recv:%s\n",recv_buf);	
	
	
	close(sockfd);

    return 0;
}
