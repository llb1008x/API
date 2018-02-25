#include<stdio.h>
#include<asm/hardware.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<string.h>
#include"led_driver.h"

int parse(char *data);
int search(char *zoom_n);

int main()
{
	char *data;	
	int fd;
	int len;
	int led = 0x00;
	
	printf("%s\r\n\r\n","Content-Type:text/html");
	len = atoi(getenv("CONTENT_LENGTH"));
	if(len != 0)
	{
		data = (char *)malloc(len);
		read(0,data,len);
		led = parse(data);
	}
		
	fd = open("/dev/gpf/0", O_RDWR);	
	if(fd == 0)
	{
		perror("fail: open /dev/gpf0\n");
		exit(1);
	}
	
	ioctl(fd, GPIO_SET_MULTI_PIN_OUT, 0x3f);
	printf("<p>led = %x\n",led);
	write(fd, &led, sizeof(led));
	free(data);
	close(fd);
	printf("<p>成功 2 秒后返回<meta content=\"2; URL=/led.html\" http-equiv=\"refresh\">");
	return 0;
}
int parse(char *data)
{
	char *zoom = NULL;
	int led = 0x00;
	int led_n = 0x00;
	
	zoom = strtok(data, "&");
	if(zoom != NULL)
	{
		led_n = search(zoom);	
		led |= led_n; 
		while(zoom = strtok(NULL, "&"))
		{
			led_n = search(zoom);
			led |= led_n;
		}
	}
	return led;
}
int search(char *zoom)
{
	char *p = NULL;
	if((p = memchr(zoom, '1', 5)) != NULL)
		return 0x01;
	else if((p = memchr(zoom, '2', 5)) != NULL)
		return 0x02;
	else if((p = memchr(zoom, '3', 5)) != NULL)
		return 0x04;
	else if((p = memchr(zoom, '4', 5)) != NULL)
		return 0x08;
	else if((p = memchr(zoom, '5', 5)) != NULL)
		return 0x10;
	else if((p = memchr(zoom, '6', 5)) != NULL)
	{
		printf("0x20\n");
		return 0x20;
	}
	else 
		return -1;
}



