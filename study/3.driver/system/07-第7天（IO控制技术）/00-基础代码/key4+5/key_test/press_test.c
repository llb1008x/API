#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
	unsigned char key = 0;
	int fp;

	fp = open("/dev/key4+5", O_RDWR);
	if(fp <0)
	{
		printf("open failed\n");
		return -1;
	}
	
	while(1)
	{
		read(fp, &key, sizeof(key)); //´ø×èÈû¶ÁÈ¡
		printf("the key = %d\n",key);
	}
	close(fp);
	return 0;
}
