/*************************************************************************
	> File Name: led.c
	> Author: 
	> Mail: 
	> Created Time: 2018年02月14日 星期三 15时46分16秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define LED_NUM 2
#define LED_C   2


int main(int argc, char **argv)
{
    int fd,led_num,led_c;
    char *leds="/dev/leds";
    
    led_num=LED_NUM;
    led_c=LED_C;
    
    printf("argv1 is cmd;argv2 is io\n");
    if(atoi(argv[1])>led_c){
        
        printf("argv1 is 0 or 1\n");
        exit(1);
    }
    
    if(atoi(argv[2])>led_num){
        
        printf("argv2 is 0 or 1\n");
        exit(1);    
    }
    
    fd=open(leds,O_RDWR|O_NOCTTY|O_NDELAY);
    if(fd<0){
        
        printf("open %s failed \n",leds);
    }else {
    
        ioctl(fd,atoi(argv[1]),atoi(argv[2]));
        printf("ioctl %s success \n",leds);
        
    }
    
    close(fd);
    
    
    return 0;
}
