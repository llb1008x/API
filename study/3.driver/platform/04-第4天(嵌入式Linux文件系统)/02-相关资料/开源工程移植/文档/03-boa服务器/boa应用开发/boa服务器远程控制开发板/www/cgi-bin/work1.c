#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include "init.h"

int main(void)
{
	int  fd;
	char *data;
	int  len;
	
	printf("%s\r\n\r\n","Content-Type:text/html");
	printf("���ñ�����,���Ժ�...\n");
	
	UartInit();//��ʼ������
	GPRSInit();//��ʼ��GPRSģ��
	
	len = atoi(getenv("CONTENT_LENGTH"));
	if(len != 0)
	{			
	    //printf("<p>%d\n", len);
		data = (char *) malloc(len);
		read(0,data,len);
		if((fd=open("parameter",O_RDWR|O_CREAT)) == 0)
		{
			printf("<p>��������ʧ��,�����±���!\n");
			exit(1);
		}
	//printf("<p>%s\n", data);
		write(fd,data,len);
		close(fd);
		printf("<p>���óɹ� 2 ��󷵻�<meta content=\"2; URL=/work1.html\" http-equiv=\"refresh\">");
	}
}
					
			
		
	

			
