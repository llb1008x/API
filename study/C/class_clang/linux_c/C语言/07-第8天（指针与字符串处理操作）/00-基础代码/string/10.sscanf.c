#include <stdio.h>
#include <string.h>
int main()
{
	char buf[20];
	sscanf("1234 5678", "%*d %s", buf);
	printf("buf1=%s\n",buf);
	sscanf("12345678", "%4s", buf);
	printf("buf2=%s\n",buf);
	sscanf("123abcABC", "%[1-9a-z]", buf);
	printf("buf3=%s\n",buf);
	sscanf("123abcABC", "%[^A-Z]", buf);
	printf("buf4=%s\n",buf);
	sscanf("123456 abcdedf", "%[^ ]", buf);
	printf("buf5=%s\n",buf);
	return 0;
}