#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

/************************************************************************
函数名称：	void * client_process(void *arg)
函数功能：	通过线程创建webserver
函数参数：	void *arg，已连接套接字
函数返回：	无
************************************************************************/
void * client_process(void *arg)
{
	int connfd = (int)arg; 
	int fd = 0;
	int len = 0;
	char recv_buf[1024] = "";
	char file_buf[1024] = "";
	char filename[50] = "html/";
	
	char head[] = "HTTP/1.1 200 OK\r\n"						\
				  "Content-Type: text/html\r\n"				\
				  "\r\n";	                               
				  
	printf("connfd=%d\n",connfd);          
	
	recv(connfd, recv_buf, sizeof(recv_buf), 0);											//接收请求
	
	sscanf(recv_buf, "GET /%[^ ]", (filename + 5));								//获取文件名
	if(filename[5] == 0)         //判断如果没有传入文件名，则使用默认的文件
	{
		strcpy(filename, "html/index.html");
	}
	
	printf("filename=*%s*\n",filename);
	
	fd = open(filename, O_RDONLY);
	if(fd < 0)									//打开文件失败
	{
		char err[]=	"HTTP/1.1 404 Not Found\r\n"			\
					"Content-Type: text/html\r\n"			\
					"\r\n"								\
					"<HTML><BODY>File not found</BODY></HTML>";             
					
		perror("open");	
		send(connfd, err, strlen(err), 0);         //发送失败的报文头
		
		close(connfd);
		return NULL;          //结束线程
	}	
	
	send(connfd, head, strlen(head), 0);         //发送成功的报文头
	
	while((len = read(fd, file_buf, sizeof(file_buf))) > 0)		//循环读取并发送文件
	{
		send(connfd, file_buf, len, 0);
	}
	
	close(fd);
	close(connfd); 
	return NULL;
}

/************************************************************************
函数名称：	int main(int argc, char *argv[])
函数功能：	主函数，建立一个web_server
函数参数：	可以传参改变自己的端口号，亦可不传参使用默认值
函数返回：	无
************************************************************************/
int main(int argc, char *argv[])
{
	unsigned short port = 8000;
	if(argc > 1)
	{
		port = atoi(argv[1]);	
	}
	
	printf("Listenning at port=%d\n",port);
	printf("Usage: http://172.20.223.16:%d\n",port);
	
	
	
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
	{
		perror("socket");	
		exit(-1);
	}
	
	struct sockaddr_in my_addr;
	bzero(&my_addr, sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port   = htons(port);
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	int err_log = bind(sockfd, (struct sockaddr*)&my_addr, sizeof(my_addr));
	if(err_log != 0)
	{
		perror("bind");
		close(sockfd);		
		exit(-1);
	}
	
	err_log = listen(sockfd, 10);
	if(err_log != 0)
	{
		perror("listen");
		close(sockfd);		
		exit(-1);
	}

	while(1)
	{
		
		char cli_ip[INET_ADDRSTRLEN] = "";
		struct sockaddr_in client_addr;
		socklen_t cliaddr_len = sizeof(client_addr);
		
		int connfd = accept(sockfd, (struct sockaddr*)&client_addr, &cliaddr_len);
		if(connfd < 0)
		{
			perror("accept this time");
			continue;
		}
		else
		{
			inet_ntop(AF_INET, &client_addr.sin_addr, cli_ip, INET_ADDRSTRLEN);
			printf("connected form %s\n\r", cli_ip);
			//为每一个请求创建一个线程为其服务
			pthread_t tid;
			pthread_create(&tid, NULL, client_process, (void *)connfd);							
			//分离线程，当线程结束时，系统会自动释放其资源
			pthread_detach(tid);	
		}
	}
	
	close(sockfd);   //关闭监听套接字
	printf("exit main!\n");
	return 0;
}
