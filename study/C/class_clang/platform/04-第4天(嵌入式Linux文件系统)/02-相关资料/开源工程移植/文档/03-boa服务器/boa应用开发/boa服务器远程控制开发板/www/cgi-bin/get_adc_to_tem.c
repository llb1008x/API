#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/types.h>
#include <fcntl.h>
#include "s3c2410-adc.h"

int main(int argc, char **argv)
{
    int fd;
		int fd1;
		int ret;
    int data;
    char buf[512];
    int i;
    int temmax;
    int temmin;
		printf("Content-Type: text/html;charset=GB2312\r\n\r\n");
    fd = open("/dev/adc/0",O_RDWR);
    if(fd<0)
  	  printf("<p>open /dev/adc/0 error");
    ret = ioctl(fd,ADC_SET_CHANNEL,1);
    if(ret<0)
    	printf("<p>ioctl set adc channel error\n");
    ret = read(fd,&data,4);
    if(ret<0)
     	printf("<p>read adc error\n");
    
	fd1 = open("parameter", O_RDONLY);
	if(fd1 < 0)
		printf("<p><h1 align=center><font color=Red>没有设置参数,请设置</h1></font>\n");
	else
	{
		printf("<h1 align=center>实时温度：%d 摄氏度</h1>\n",data);
		ret = read(fd1, buf, 512);
		sscanf(buf, "set_tem_max=%d&set_tem_min=%d", &temmax, &temmin);
		printf("<p>temmax:%d temmin:%d data:%d\n",temmax, temmin, data);
		if(data > temmax)
			printf("<p><h1 align=center><font color=Red>警报：温度过高!</h1></font>\n");
		else if(data < temmin)
			printf("<p><h1 align=center><font color=Red>警报：温度过低!</h1></font>\n");
		else
			printf("<p><h1 align=center><font color=Green>正常</h1></font>\n");
		close(fd1);
		return 0;
	}
}		
