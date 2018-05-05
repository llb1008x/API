#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	printf("%s\r\n\r\n","Content-Type:text/html");
	printf("<html>\n<TITLE>CGI1:CGI hello!</TITLE>\n");
	printf("<center><H1>hello, this is frist CGI demo!</H1></center>\n</html>");
	return 0;
}
