/* ************************************************************************
 *       Filename:  8_udp_多播.c
 *    Description:  
 *        Version:  1.0
 *        Created:  2015年07月22日 11时47分00秒
 *       Revision:  none
 *       Compiler:  gcc
 *         Author:  YOUR NAME (), 
 *        Company:  
 * ************************************************************************/

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
	//发送时是发送给多播地址的，不是单个IP
	dst_addr.sin_addr.s_addr = inet_addr("224.0.1.1");
	
	char send_buf[10]="hello";
	sendto(sockfd,send_buf,5,0,(struct sockaddr *)&dst_addr,sizeof(dst_addr));
	
	return 0;
}


