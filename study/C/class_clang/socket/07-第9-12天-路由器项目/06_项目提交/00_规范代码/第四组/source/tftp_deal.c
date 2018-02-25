#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "tftp_deal.h"

/******************************************************************
功	能:	上传/下载处理
参	数:	无
返回值: 无
*******************************************************************/
void tftp_deal()
{
	char ip_buf[100] = "";
	char file_name[100] = "";
	char temp_buf[200] = "";
	int cmd = 0;
	
	printf("1.下载\n2.上传\n");
	scanf("%d", &cmd);
	if(cmd == 1)//下载
	{
		printf("请输入您要下载的地址和文件,如10.220.4.1 hello.txt\n");
		getchar();
		fgets(temp_buf, sizeof(temp_buf), stdin);
		sscanf(temp_buf, "%[^ ] %s", ip_buf, file_name);
		download(ip_buf, file_name);
	}
	else if(cmd == 2)//上传
	{
		printf("请输入您要上传的地址和文件,如10.220.4.1 hello.txt\n");
		getchar();
		fgets(temp_buf, sizeof(temp_buf), stdin);
		sscanf(temp_buf, "%[^ ] %s", ip_buf, file_name);
		upload(ip_buf, file_name);
	}
	else
	{
		printf("输入无效。\n");
	}
}

/******************************************************************
功	能:	下载文件
参	数:	下载的目的ip地址，待下载的文件名称
返回值: 无
*******************************************************************/
void download(char *ip_buf, char *file_name)
{
	/*创建套接字*/
	int sock_fd = 0;
	sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock_fd < 0)
	{
		perror("socket");
		exit(-1);
	}
	
	/*目的地址结构体*/
	struct sockaddr_in dest_addr;
	bzero(&dest_addr, sizeof(dest_addr));
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(69);
	inet_pton(AF_INET, ip_buf, &dest_addr.sin_addr);
	
	/*组TFTP包*/
	char send_buf[200] = "";
	int buf_len = sprintf(send_buf, "%c%c%s%c%s%c", 0, 1, file_name, 0, "octet", 0);
	
	/*发送下载请求*/
	sendto(sock_fd, send_buf, buf_len, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));

	/*服务器地址结构体*/
	struct sockaddr_in server_addr;
	socklen_t sock_len = sizeof(server_addr);

	/*打开文件*/
	int fd = open(file_name, O_RDWR | O_CREAT | O_TRUNC, 0666);//以读写方式打开文件，若文件不存在，则创建该文件，
															//若文件存在，则清空文件
	if(fd < 0)
	{
		perror("open");
	}
	
	/*下载文件*/
	int len_recv = 0;
	char recv_buf[516] = "";
	char ack_buf[4] = "";
	do
	{
		len_recv = recvfrom(sock_fd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&server_addr, &sock_len);//接收数据
		/*判断接收包的类型*/
		if(recv_buf[1] == 3)//数据包
		{
			write(fd, recv_buf+4, len_recv-4);											//写入文件
			sprintf(ack_buf, "%c%c%c%c", 0, 4, recv_buf[2],recv_buf[3]);				//组ack包
			sendto(sock_fd, ack_buf, 4, 0, (struct sockaddr *)&server_addr, sock_len);	//发送ack包
		}
		else if(recv_buf[1] == 5)//错误信息包
		{
			printf("%s\n",recv_buf+4);	//打印错误信息
			remove(file_name);			//删除文件
			close(fd);					//关闭文件
			close(sock_fd);				//关闭套接字
			exit(-1);					//退出
		}
	}while(len_recv == 516);
	
	close(fd);//关闭文件
	close(sock_fd);//关闭套接字
	printf("下载完成\n");
	
}

/******************************************************************
功	能:	上传文件
参	数:	上传的目的ip地址，待上传的文件名称
返回值: 无
*******************************************************************/
void upload(char *ip, char *file_name)
{
	/*创建套接字*/
	int sock_fd = 0;
	sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock_fd < 0)
	{
		perror("socket");
		exit(-1);
	}
	
	/*目的地址结构体*/
	struct sockaddr_in  dest_addr;
	bzero(&dest_addr,sizeof(dest_addr));

	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(69);
	inet_pton(AF_INET, ip, &dest_addr.sin_addr);
	
	/*组TFTP包*/
	char send_buf[200] = "";
	int buf_len = sprintf(send_buf, "%c%c%s%c%s%c",0,2,file_name,0,"octet",0);
	
	/*发送上传请求*/
	sendto(sock_fd, send_buf, buf_len, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
	
	/*服务器地址结构体*/
	struct sockaddr_in  server_addr;
	socklen_t sock_len = sizeof(server_addr);

	/*打开文件*/
	int fd;
	fd = open(file_name, O_RDONLY); //只读方式打开文件		
	if(fd < 0)
	{
		perror("open");
	}
	
	/*上传文件*/
	int len_read = 0;
	char recv_buf[516] = "";
	unsigned int num1 = 1;
	unsigned int num2 = 0;
	do
	{
		recvfrom(sock_fd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&server_addr, &sock_len);//接收数据
		/*判断接收包的类型*/
		if(recv_buf[1] == 4)//ACK包
		{
			//方法一：
			if(num1 == 256)
			{
				num2++;
				num1 = 0;
			}
			recv_buf[3] = num1;
			recv_buf[2] = num2;
			num1++;
			
			recv_buf[1] = 3;
			len_read = read(fd, recv_buf+4, sizeof(recv_buf)-4);//读取文件
			sendto(sock_fd, recv_buf, len_read+4, 0, (struct sockaddr *)&server_addr, sock_len);//发送数据包
		}
		else if(recv_buf[1] == 5)//错误信息包
		{
			printf("%s\n", recv_buf+4);//打印错误信息
			close(fd);//关闭文件
			close(sock_fd);//关闭套接字
			exit(-1);//退出
		}
	}while(len_read == 512);

	printf("上传成功！\n");
	close(fd);//关闭文件
	close(sock_fd);//关闭套接字
}

