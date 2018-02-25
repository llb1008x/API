#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

/************************************************************************
函数名称：	int main(int argc, char *argv[])
函数功能：	主函数, 进程实现QQ聊天
函数参数：	无
函数返回：	无
************************************************************************/
int main(int argc,char *argv[])
{
	unsigned short port = 8000;
	struct sockaddr_in my_addr;
	struct sockaddr_in dest_addr;
	
	//对套接字地址进行初始化
	bzero(&my_addr, sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(port);
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	bzero(&dest_addr, sizeof(dest_addr));	
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(8000);//对方的端口号

	//创建套接口
	int sockfd = socket(AF_INET,SOCK_DGRAM, 0);
	if( sockfd < 0)
	{
		perror("socket");
		exit(-1);
	}
	
	//设置端口
	if(bind(sockfd, (struct sockaddr*)&my_addr, sizeof(my_addr)) != 0)
	{
		perror("bind");
		close(sockfd);		
		exit(-1);
	}
	
	printf("input: \"sayto 192.168.221.X\" to sendmsg to somebody\n");
	//创建接收进程
	pid_t pid = fork();
	if(pid < 0)
	{
		perror("fork\n");
		exit(-1);
	}
	if(pid == 0)
	{
		//子进程
		struct sockaddr_in client_addr;
		socklen_t cliaddr_len = sizeof(client_addr);
		
		while(1)
		{			
			char recv_buf[1024] = "";
			char cli_ip[INET_ADDRSTRLEN] = "";
			
			recvfrom(sockfd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr*)&client_addr, &cliaddr_len);
			inet_ntop(AF_INET, &client_addr.sin_addr, cli_ip, INET_ADDRSTRLEN);
			
			printf("\r\033[31m[%s]:\033[32m%s\n", cli_ip, recv_buf);
			write(1, "UdpQQ:", 6);
		}
		close(sockfd);
		exit(0);
	}

	while(1)
	{	
		//主进程负责发送消息
		char send_buf[1024] = "";
		
		write(1,"UdpQQ:",6);		//1 表示标准输出
		fgets(send_buf, sizeof(send_buf), stdin); 	//等待输入
		send_buf[strlen(send_buf) - 1] = '\0'; 	//确保输入的最后一位是'\0'
		
		if(strncmp(send_buf, "sayto", 5) == 0)
		{
			char aim_ip[INET_ADDRSTRLEN] = "";
			
			inet_pton(AF_INET, send_buf+6, &dest_addr.sin_addr);//给addr套接字地址再赋值.
			
			inet_ntop(AF_INET, &dest_addr.sin_addr, aim_ip, INET_ADDRSTRLEN);
			printf("\rconnect %s successful!\n", aim_ip);
			continue;
		}
		else if(strcmp(send_buf, "exit") == 0)
		{
			close(sockfd);
			exit(0);
		}
		sendto(sockfd, send_buf, strlen(send_buf), 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr));
	}
	
	close(sockfd);
	return 0;
}


