#include<stdio.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include"s5pv210-gpio.h"
#include<fcntl.h>
int main()
{
	int gph3_fd,gph2_fd;
	int temp;
	int key;
	gph3_fd=open("/dev/gpH3",O_RDWR);
	ioctl(gph3_fd,GPIO_SET_PIN_OUT,0);
	temp=0;
	ioctl(gph3_fd,GPIO_CLR_PIN,0);
	close(gph3_fd);
	
	
	
	gph3_fd=open("/dev/gpH3",O_RDWR);
	ioctl(gph3_fd,GPIO_SET_PIN_OUT,0);
	temp=0;
	ioctl(gph3_fd,GPIO_CLR_PIN,0);
	close(gph3_fd);
	
	gph2_fd=open("/dev/gpH2",O_RDWR);
	ioctl(gph2_fd,GPIO_SET_PIN_IN,3);
	ioctl(gph2_fd,GPIO_SET_PIN_IN,4);
	ioctl(gph2_fd,GPIO_SET_PIN_IN,5);
	ioctl(gph2_fd,GPIO_SET_PIN_IN,6);
	ioctl(gph2_fd,GPIO_SET_PIN_IN,7);
	
	ioctl(gph2_fd,GPIO_SET_PULL_UP,3);
	ioctl(gph2_fd,GPIO_SET_PULL_UP,4);
	ioctl(gph2_fd,GPIO_SET_PULL_UP,5);
	ioctl(gph2_fd,GPIO_SET_PULL_UP,6);
	ioctl(gph2_fd,GPIO_SET_PULL_UP,7);
	printf("test0000000000000000\n");
	while(1)
	{
	
		read(gph2_fd,&temp,4);
		temp &=0xF8;
		
		if(temp!=0xF8)
		{
			
			switch(temp)
			{
				case 0xF0:
					key=2;
					break;
				case 0xE8:
					key=3;
					break;
				case 0xD8:
					key=4;
					break;
				case 0xB8:
					key=5;
					break;
				case 0x78:
					key=6;
					break;
				default :
					break;
			}
		
			do{//等待键盘抬起
			
				read(gph2_fd,&temp,4);
				temp &=0xF8;
			}while(temp!=0xF8);
		
		}
		else
		{
			
			key=-1;
		}
		
		if(key!=-1)
			printf("key%d被按下\n",key);
	}
	close(gph2_fd);
	return 0;
}