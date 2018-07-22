#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/************************************************************************
�������ƣ�	int main(int argc, char *argv[])
�������ܣ�	ͨ�����̴���webserver
����������	int argc, char *argv[]
�������أ�	��
************************************************************************/
int main(int argc, char *argv[])
{
	unsigned short port = 8000;   //����Ĭ�϶˿ں�
	if(argc > 1)
	{
		port = atoi(argv[1]);	//������2��ֵ���˿ںű���
	}
	
	//����TCP�׽���
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if( sockfd < 0)
	{
		perror("socket");	
		exit(-1);
	}
	
	//�������׽��ֵ�ַ������ֵ
	struct sockaddr_in my_addr;
	bzero(&my_addr, sizeof(my_addr));
	my_addr.sin_family = AF_INET;   //IPV4��
	my_addr.sin_port   = htons(port); //���˿ں�ת���������ֽ���
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY); //����IP��ַ
	
	//��TCP�׽���
	if( bind(sockfd, (struct sockaddr*)&my_addr, sizeof(my_addr)) != 0)
	{
		perror("bind");
		close(sockfd);		
		exit(-1);
	}
	
	//����
	if( listen(sockfd, 10) != 0)
	{
		perror("listen");
		close(sockfd);		
		exit(-1);
	}

	printf("Listenning at port=%d\n",port);   //��ӡ�˿ں���Ϣ
	printf("Usage: http://172.20.223.16:%d/html/index.html\n",port);
	
	while(1)
	{
		char cli_ip[INET_ADDRSTRLEN] = "";  //��ſͻ��˵��ʮ����IP��ַ
		struct sockaddr_in client_addr;
		socklen_t cliaddr_len = sizeof(client_addr);
		
		//�ȴ��ͻ�������
		int connfd = accept(sockfd, (struct sockaddr*)&client_addr, &cliaddr_len);
		printf("connfd=%d\n",connfd); //��ӡ�������׽���
		if(connfd > 0)
		{
			if(fork() == 0)  //�������̲��жϷ���ֵ
			{
				close(sockfd);
				//�ӽ���ִ��
				int  fd = 0;
				int  len = 0;
				char buf[1024] = "";
				char filename[50] = "";
				
				//���ճɹ�ʱ���ص�ͷ��
				char head[]="HTTP/1.1 200 OK\r\n"					\
							"Content-Type: text/html\r\n"		\
							"\r\n";
				
				//�������ֽ���ת���ɵ��ʮ������ʽ�����cli_ip��
				inet_ntop(AF_INET, &client_addr.sin_addr, cli_ip, INET_ADDRSTRLEN);
				printf("connected form %s\n\r", cli_ip);   //��ӡ���ʮ������ʽ�Ŀͻ���IP��ַ
				recv(connfd, buf, sizeof(buf), 0);   //���տͻ��˷��͵���������
				sscanf(buf, "GET /%[^ ]", filename);   //�����ͻ��˷��������ַ���
				printf("filename=*%s*\n", filename);
				
				fd = open(filename, O_RDONLY);   //��ֻ����ʽ���ļ�
				if( fd < 0)   //������ļ�ʧ��
				{
					//HTTPʧ��ͷ��
					char err[]=	"HTTP/1.1 404 Not Found\r\n"		\
								"Content-Type: text/html\r\n"		\
								"\r\n"								\
								"<HTML><BODY>File not found</BODY></HTML>";
					perror("open error");					
					send(connfd, err, strlen(err), 0);
					close(connfd);  //�ر��������׽���
					exit(0);	//�ӽ����˳�
				}	
				//���ļ��ɹ���
				send(connfd, head, strlen(head), 0);  //����HTTP����ɹ�ͷ��
				while( (len = read(fd, buf, sizeof(buf))) > 0)   //ѭ����ȡ�ļ�����
				{
					send(connfd, buf, len, 0);       //�����õ����ݷ��͸��ͻ���
				}
				
				close(fd);   //�ɹ���ر��ļ�
				close(connfd);   //�ر��������׽���
				exit(0);	 //�ӽ����˳�
			}
		}	
		
		close(connfd);   //�����̹ر������׽���
	}
	close(sockfd);
	printf("exit main!\n");
	return 0;
}
