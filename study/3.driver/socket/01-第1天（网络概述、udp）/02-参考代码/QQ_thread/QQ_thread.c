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
函数名称：	void *Recv_Thread(void* arg)
函数功能：	线程函数，负责接收对方的信息，并打印
函数参数：	arg套接字
函数返回：	无
************************************************************************/
void *Recv_Thread(void* arg)
{
	int sockfd = (int)arg;
	struct sockaddr_in client_addr;
	socklen_t cliaddr_len = sizeof(client_addr);
	
	while(1)
	{
		char recv_buf[200]  = "";
		char cli_ip[INET_ADDRSTRLEN] = "";
		
		recvfrom(sockfd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr*)&client_addr, &cliaddr_len);
		inet_ntop(AF_INET, &client_addr.sin_addr, cli_ip, sizeof(cli_ip));
		printf("\r\033[31m[%s]:\033[32m%s\n", cli_ip, recv_buf);
		write(1,"UdpQQ:",6);
	}
	return NULL;
}

/************************************************************************
函数名称：	int main(int argc, char *argv[])
函数功能：	主函数，用线程实现聊天
函数参数：	无
函数返回：	无
************************************************************************/
int main(int argc, char *argv[])
{
	struct sockaddr_in my_addr; 
	struct sockaddr_in dest_addr;
	
	//对套接字地址进行初始化
	bzero(&my_addr, sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port   = htons(8000);
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	bzero(&dest_addr, sizeof(dest_addr));	
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port   = htons(8000);

	//创建套接口
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if( sockfd < 0)
	{
		perror("socket error");
		exit(-1);
	}
	
	//设置端口
	int err_log = bind(sockfd, (struct sockaddr*)&my_addr, sizeof(my_addr));
	if( err_log < 0)
	{
		perror("bind");
		close(sockfd);		
		exit(-1);
	}
	
	printf("input: \"sayto 192.168.221.X\" to sendmsg to somebody\n");
	//创建接收线程
	pthread_t tid;
	pthread_create(&tid, NULL, Recv_Thread, (void*)sockfd); //创建线程
	printf("\033[32m"); //设置字体颜色
	fflush(stdout);
	
	//主进程负责发送消息
	while(1)
	{	
		write(1, "UdpQQ:", 6);     //1 表示标准输出
		char send_buf[512] = "";
		
		fgets(send_buf, sizeof(send_buf), stdin); //等待输入
		send_buf[strlen(send_buf) - 1] = '\0';    //确保输入的最后一位是'\0'
		if(strncmp(send_buf, "sayto", 5) == 0)
		{
			inet_pton(AF_INET, send_buf+6, &dest_addr.sin_addr);   //给addr套接字地址再赋值
			printf("\rconnect %s successful!\n", send_buf+6);
			continue;
		}
		else if(strncmp(send_buf, "exit", 4) == 0)
		{
			close(sockfd);
			exit(0);
		}
		sendto(sockfd, send_buf, strlen(send_buf), 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr));
	}
	return 0;
}
