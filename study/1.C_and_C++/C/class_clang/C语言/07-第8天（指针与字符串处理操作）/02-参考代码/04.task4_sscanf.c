#include <stdio.h>
int main()
{
	char buf[20];
	sscanf("abc#def@ghi", "%*[^#]#%[^@]", buf);
	printf("%s\n", buf);
	return 0;
}

