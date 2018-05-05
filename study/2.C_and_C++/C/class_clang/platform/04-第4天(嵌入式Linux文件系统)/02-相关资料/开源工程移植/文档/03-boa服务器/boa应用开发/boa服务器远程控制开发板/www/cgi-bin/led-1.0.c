#include<stdio.h>
#include<asm/hardware.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/stat.h>

#include"led_driver.h"

int main()
{
	int i;
	char *data;
	int fd;
	int len;
	int led;
	int led1=0x0;
	int led2=0x0;
	int led3=0x0;
	int led4=0x0;
	int led5=0x0;
	int led6=0x0;
	printf("%s\r\n\r\n","Content-Type:text/html");
	len = atoi(getenv("CONTENT_LENGTH"));
	if(len != 0)
		{
			data = (char *)malloc(len);
			read(0,data,len);

			sscanf(data,"zoom1=%x&zoom2=%x&zoom3=%x&zoom4=%x&zoom5=%x&zoom6=%x",
				&led1,&led2,&led3,&led4,&led5,&led6);
			printf("<p>data= %x\n",data);
		}
		
	fd = open("/dev/gpf/0", O_RDWR);	
	if(fd == 0)
	{
		perror("fail: open /dev/gpf0\n");
		exit(1);
	}
	
	ioctl(fd, GPIO_SET_MULTI_PIN_OUT, 0xff);
	led = (led1 | led2 | led3 | led4 | led5 | led6);
	printf("<p>led = %x\n",led);
	write(fd, &led, sizeof(led));
	close(fd);
	
	return 0;
}
	
