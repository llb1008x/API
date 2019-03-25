#include <stdio.h>
#include <string.h>

int main()
{
	int mic[10],sec[10],msec[10];
	char str[] = "[02:06.85]",out[10];
	sscanf(str,"%*[^[][%[0-9]",mic);
	printf("%s,%s\n",mic,sec);
	return 0;
}
