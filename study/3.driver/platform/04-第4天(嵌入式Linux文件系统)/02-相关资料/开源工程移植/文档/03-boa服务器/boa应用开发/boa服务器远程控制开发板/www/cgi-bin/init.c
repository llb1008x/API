#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<fcntl.h>
#include<assert.h>
#include<termios.h>
#include<sys/stat.h>
#include<sys/time.h>

#include"init.h"

void UartInit()
{
	int g_fd_serial;
	struct termios newtio, oldtio;

	g_fd_serial = open("/dev/ttyS0", O_NOCTTY|O_RDWR);
	if(g_fd_serial < 0)
		printf("open /dev/ttyS0 error\n");
	tcgetattr(g_fd_serial, &oldtio);
	bzero(&newtio, sizeof(newtio));

	newtio.c_cflag = B115200 | CS8;
	newtio.c_iflag = IGNPAR | ICRNL;
	newtio.c_oflag = 0;
	newtio.c_lflag  = ICANON;
	newtio.c_lflag  &= ~(ICANON | ECHO | ECHOE | ISIG);
	newtio.c_oflag  &= ~OPOST;

	tcflush(g_fd_serial, TCIFLUSH);
	tcsetattr(g_fd_serial, TCSANOW, &newtio);
	close(g_fd_serial);
}

void GPRSInit()
{
	int fd_serial;
	char buf[50];
	fd_serial = open("/dev/ttyS0", O_NOCTTY|O_RDWR);
	if(fd_serial < 0)
		printf("open /dev/ttyS0 error\n");
	printf("<p>GPRS 初始化中.....\n");
	strcpy(buf, "AT+CFUN=1\r\n");
	write(fd_serial, buf, strlen(buf));//GPRS 复位
	sleep(1);
	strcpy(buf, "AT+CMIC=0,15\r\n");
	write(fd_serial, buf, strlen(buf));//麦克设置
	sleep(1);
	strcpy(buf, "AT+CMGF=1\r\n");
	write(fd_serial, buf, strlen(buf));//设置短信格式为TEXT 
	sleep(1);
	strcpy(buf, "AT+CHFA=1\r\n");
	write(fd_serial, buf, strlen(buf));//耳机设置
	sleep(1);
	strcpy(buf, "AT+CLVL=100\r\n");
	write(fd_serial, buf, strlen(buf));//设置音量为最大
	sleep(1);
	strcpy(buf, "AT+CSMP=17,255,0,8\r\n");
	write(fd_serial, buf, strlen(buf));//设置 TEXT 方式参数
	sleep(1);
	strcpy(buf, "AT+CSCS=\"UCS2\"\r\n");
	write(fd_serial, buf, strlen(buf));//USC2设置
	sleep(1);
	printf("<p>GPRS 初始完成\n");
	strcpy(buf, "ATD112\r\n");
	
	write(fd_serial, buf, strlen(buf));
	sleep(10);
	strcpy(buf, "ATH\r\n");
	write(fd_serial, buf, strlen(buf));
	
	close(fd_serial);
}
