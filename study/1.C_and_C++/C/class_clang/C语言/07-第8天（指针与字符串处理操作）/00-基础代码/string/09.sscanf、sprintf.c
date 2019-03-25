#include <stdio.h>
#include <string.h>
int main()
{
	char buf[20];
	int a, b, c;
	
	sprintf(buf,"%d:%d:%d",2013,10,1);
	printf("buf=%s\n",buf);
	
	sscanf("2013:10:1", "%d:%d:%d", &a, &b, &c);
	printf("a=%d,b=%d,c=%d\n",a,b,c);
	return 0;
}