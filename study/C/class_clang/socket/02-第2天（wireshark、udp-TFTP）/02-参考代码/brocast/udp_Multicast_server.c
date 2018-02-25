#include <unp.h>
int main(int argc, char *argv[])
{
	
	int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0)
	{
		perror("socket");
		exit(-1);
	}
	
	//多播IP地址
	char group_ip[16]="224.0.1.1";
	//多播地址结构体
	struct ip_mreq mreq;
	
	//添加一个多播组IP
	mreq.imr_multiaddr.s_addr = inet_addr(group_ip);
	//添加一个将要添加到多播组的IP
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	
	//设置套接字选项
	setsockopt(sockfd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq));
	
	struct sockaddr_in dst_addr;
	bzero(&dst_addr,sizeof(dst_addr));
	dst_addr.sin_family = AF_INET;
	dst_addr.sin_port = htons(8081);
	dst_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		
	bind(sockfd,(struct sockaddr *)&dst_addr,sizeof(dst_addr));

	char recv_buf[10]="";
	recvfrom(sockfd,recv_buf,10,0,NULL,NULL);
	printf("recv_buf===%s\n",recv_buf);

	
	return 0;
}
