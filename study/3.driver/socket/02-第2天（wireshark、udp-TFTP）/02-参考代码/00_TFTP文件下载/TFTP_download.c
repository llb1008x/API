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
�������ܣ�	tftp�ͻ��˳��򣬿��Դ�tftp���������س���
����������	��
�������أ�	��
************************************************************************/
int main(int argc, char *argv[])
{
	unsigned short p_num = 0;
	unsigned char cmd = 0;
	char cmd_buf[512] = "";
	char recv_buf[516] = "";
	
	struct sockaddr_in client_addr;
	socklen_t cliaddr_len = sizeof(client_addr);

	if( argc < 3 )           //�����������С��3,����Ϊ�����������
	{
		printf("error cmd!\n"\
			"cmd example:./tftpc 192.168.220.24 hello.txt\n");
		return 0;
	}
	
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0)
	{
		perror("socket error");
		exit(-1);
	}
	
	struct sockaddr_in dest_addr;
	bzero(&dest_addr, sizeof(dest_addr));
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(69);	
	inet_pton(AF_INET, argv[1], &dest_addr.sin_addr);
	
	//������������,argv[2]Ϊ�ļ���
	int len = sprintf(cmd_buf, "%c%c%s%c%s%c", 0, 1, argv[2], 0, "octet", 0);	//���Ͷ����ݰ�	
	sendto(sockfd, cmd_buf, len, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
	
	int fd = open(argv[2], O_WRONLY|O_CREAT, 0666);
	if(fd < 0 )
	{
		perror("open error");
		close(sockfd);
		exit(-1);
	}
	
	do{
		//���շ��������͵�����
		len = recvfrom(sockfd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr*)&client_addr, &cliaddr_len);
		
		cmd = recv_buf[1];
		if( cmd == 3 )	//�Ƿ�Ϊ���ݰ�
		{
			//������Ƿ���ϴ����		
			if( (unsigned short)(p_num+1) == ntohs(*(unsigned short*)(recv_buf+2) ))
			{
				write(fd, recv_buf+4, len-4);
				p_num = ntohs(*(unsigned short*)(recv_buf+2));
				
				printf("recv:%d\n", p_num);//ʮ���Ʒ�ʽ��ӡ�����
			}
			
			recv_buf[1] = 4;
			sendto(sockfd, recv_buf, 4, 0, (struct sockaddr*)&client_addr, sizeof(client_addr));
		}
		else if( cmd == 5 ) //�Ƿ�Ϊ����Ӧ��
		{
			close(fd);
			close(sockfd);
			unlink(argv[2]);
			printf("error:%s\n", recv_buf+4);
			return 0;
		}		
	}while(len == 516); //����յ�������С��516����Ϊ����
	
	close(fd);
	close(sockfd);
	return 0;
}
