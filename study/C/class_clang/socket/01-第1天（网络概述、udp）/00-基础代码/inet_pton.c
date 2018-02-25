#include <stdio.h>
#include <arpa/inet.h>
int main()
{
	char ip_str[]="172.20.223.75";
	unsigned int ip_uint = 0;
	unsigned char *ip_p = NULL;

	inet_pton(AF_INET,ip_str,&ip_uint);
	printf("in_uint = %d\n",ip_uint);
	
	ip_p = (char *)&ip_uint;
	printf("in_uint = %d,%d,%d,%d\n",*ip_p,*(ip_p+1),*(ip_p+2),*(ip_p+3));

	return 0;
}