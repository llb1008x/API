#include <stdio.h>
#include <arpa/inet.h>
int main()
{
	unsigned char ip[] = {172,20,223,75};
	char ip_str[16] = "NULL";

	inet_ntop(AF_INET,(unsigned int *)ip,ip_str,16);
	printf("ip_str = %s\n",ip_str);

	return 0;
}