#include <stdio.h>
int main()
{
	char min[20];
	char sec[20];
	char msec[20];
	int min_i,sec_i;
	//sscanf("[02:06.85]", "[%d:%d", &min_i,&sec_i);
	//printf("%02d:%02d\n", min_i,sec_i);
	sscanf("[02:06.85]", "%*[^0-9]%[^:]:%[^.].%[^]]", min,sec,msec);
	printf("%s:%s:%s\n", min,sec,msec);
	return 0;
}

