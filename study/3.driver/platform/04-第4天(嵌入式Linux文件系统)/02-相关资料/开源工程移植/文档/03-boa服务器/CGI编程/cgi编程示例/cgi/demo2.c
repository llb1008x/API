#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	char *data;
	int a=0, b=0;
	data = getenv("QUERY_STRING");	
	printf("%s\r\n\r\n","Content-Type:text/html");
	printf("<html>\n<TITLE>CGI2:��ӽ��</TITLE><br>\n");
	sscanf(data, "m=%d&n=%d",&a,&b);
	printf("<H1>��ӽ��Ϊ��%d</H1>",a+b);
	return 0;
}
