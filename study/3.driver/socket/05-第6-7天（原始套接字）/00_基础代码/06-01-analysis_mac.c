#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netinet/ether.h>
int main(int argc,char *argv[])
{
	unsigned char buf[1024] = "";
	int sock_raw_fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	while(1)
	{
		unsigned char src_mac[18] = "";
		unsigned char dst_mac[18] = "";
		//��ȡ��·�������֡
		recvfrom(sock_raw_fd, buf, sizeof(buf),0,NULL,NULL);
		//��buf����ȡĿ��mac��Դmac
		sprintf(dst_mac,"%02x:%02x:%02x:%02x:%02x:%02x", \
		buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]);
		sprintf(src_mac,"%02x:%02x:%02x:%02x:%02x:%02x", \
		buf[6], buf[7], buf[8], buf[9], buf[10], buf[11]);
		//��ӡԴMAC��Ŀ��MAC
		printf("MAC:%s >> %s\n", src_mac, dst_mac);
	}
	return 0;
}


