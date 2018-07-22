#include <stdio.h> /* FILE */
#include <fcntl.h>/* O_RDWR */
#include <linux/fb.h>
#include <sys/mman.h>/* PROT_READ,PROT_WRITE,MAP_SHARED */
#include <string.h>/* memset() */
#include <asm/ioctl.h>	/* _IO */
#include <linux/input.h>	/* input_event */
#include "driverapp.h"
#include <linux/i2c-dev.h>

int main(void)
{
	int fd;
	char addr = (char)0x02;
	signed char data[6] = {0};
	signed short x, y, z;
	fd  = open("/dev/i2c-2", O_RDWR);
	if(fd < 0)
	{
		perror("open");
		return 0;
	}
	ioctl(fd, I2C_SLAVE_FORCE, 0x38);/* 指定设备地址 */
	while(1)
	{
		write(fd, &addr, sizeof(addr));/* 发寄存器地址 */
		read(fd, data, sizeof(data));/* 从寄存器中读数据 */
		
		x = (data[0] >> 6) | (data[1] << 2);
		y = (data[2] >> 6) | (data[3] << 2);
		z = (data[4] >> 6) | (data[5] << 2);
		
		printf("x=%d\ty=%d\tz=%d\n", (signed short)(x << 6) >>6,  
									 (signed short) (y << 6) >>6,  
									 (signed short)(z << 6) >>6);
		usleep(100 * 1000);
	}
	return 0;	
}

