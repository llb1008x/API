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
	printf("设置保存中,请稍后...\n");
	
	UartInit();//初始化串口
	GPRSInit();//初始化GPRS模块
	
	len = atoi(getenv("CONTENT_LENGTH"));
	if(len != 0)
	{			
	    //printf("<p>%d\n", len);
		data = (char *) malloc(len);
		read(0,data,len);
		if((fd=open("parameter",O_RDWR|O_CREAT)) == 0)
		{
			printf("<p>保存设置失败,请重新保存!\n");
			exit(1);
		}
	//printf("<p>%s\n", data);
		write(fd,data,len);
		close(fd);
		printf("<p>设置成功 2 秒后返回<meta content=\"2; URL=/work1.html\" http-equiv=\"refresh\">");
	}
}
					
			
		
	

			
