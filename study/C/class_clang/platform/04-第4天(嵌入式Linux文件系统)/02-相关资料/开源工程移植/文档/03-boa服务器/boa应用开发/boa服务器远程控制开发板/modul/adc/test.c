#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/types.h>
#include <fcntl.h>
#include "s3c2410-adc.h"

main()
{
        int fd;
        int ret;
        int data;

        fd = open("/dev/adc/0",O_RDWR);
        if(fd<0)
                printf("open /dev/adc/0 error");
        ret = ioctl(fd,ADC_SET_CHANNEL,1);
        if(ret<0)
                printf("ioctl set adc channel error\n");
        while(1)
        {
                ret = read(fd,&data,4);
                if(ret<0)
                        printf("read adc error\n");
                printf("adc is %d\n",data);
                sleep(1);
        }
}
