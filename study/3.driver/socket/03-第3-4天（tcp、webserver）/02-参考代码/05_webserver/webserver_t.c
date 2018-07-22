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
�������ƣ�	void * client_process(void *arg)
�������ܣ�	ͨ���̴߳���webserver
����������	void *arg���������׽���
�������أ�	��
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
	
	recv(connfd, recv_buf, sizeof(recv_buf), 0);											//��������
	
	sscanf(recv_buf, "GET /%[^ ]", (filename + 5));								//��ȡ�ļ���
	if(filename[5] == 0)         //�ж����û�д����ļ�������ʹ��Ĭ�ϵ��ļ�
	{
		strcpy(filename, "html/index.html");
	}
	
	printf("filename=*%s*\n",filename);
	
	fd = open(filename, O_RDONLY);
	if(fd < 0)									//���ļ�ʧ��
	{
		char err[]=	"HTTP/1.1 404 Not Found\r\n"			\
					"Content-Type: text/html\r\n"			\
					"\r\n"								\
					"<HTML><BODY>File not found</BODY></HTML>";             
					
		perror("open");	
		send(connfd, err, strlen(err), 0);         //����ʧ�ܵı���ͷ
		
		close(connfd);
		return NULL;          //�����߳�
	}	
	
	send(connfd, head, strlen(head), 0);         //���ͳɹ��ı���ͷ
	
	while((len = read(fd, file_buf, sizeof(file_buf))) > 0)		//ѭ����ȡ�������ļ�
	{
		send(connfd, file_buf, len, 0);
	}
	
	close(fd);
	close(connfd); 
	return NULL;
}

/************************************************************************
�������ƣ�	int main(int argc, char *argv[])
�������ܣ�	������������һ��web_server
����������	���Դ��θı��Լ��Ķ˿ںţ���ɲ�����ʹ��Ĭ��ֵ
�������أ�	��
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
			//Ϊÿһ�����󴴽�һ���߳�Ϊ�����
			pthread_t tid;
			pthread_create(&tid, NULL, client_process, (void *)connfd);							
			//�����̣߳����߳̽���ʱ��ϵͳ���Զ��ͷ�����Դ
			pthread_detach(tid);	
		}
	}
	
	close(sockfd);   //�رռ����׽���
	printf("exit main!\n");
	return 0;
}
