/*************************************************************************
	> File Name: serial.c
	> Author: 
	> Mail: 
	> Created Time: 2018年02月14日 星期三 16时48分39秒
	
串口的发送和接收,初始化	读串口
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>

#define UART_NAME   "/dev/ttySAC3"
#define TRANSPORT_BYTES     115200

int set_opt(int ,int ,int ,char ,int );

int main(int argc,char **argv)
{
    int fd,wr_static,i=10;
    char *uart3=UART_NAME;
    char *buffer="hello world!\n";
    
    printf("\r\nitop4412 uart3 writetest start\r\n");
    
    fd=open(uart3,O_RDWR|O_NOCTTY|O_NDELAY);
    if(fd<0){
    
        printf("open %s is failed",uart3);
    }else {
    
        printf("open %s is success \n",uart3);
    }
    
    set_opt(fd,TRANSPORT_BYTES,8,'N',1);
    while(i--){
    
        wr_static=write(fd,buffer,strlen(buffer));
        if(wr_static<0){
            printf("write failed\n");
        }else{ 
            printf("wr_static is %d\n",wr_static);
        }
        
        sleep(1);
    }
    
    close(fd);
    return 0;
}



int set_opt(int fd,int nSpeed,int nBits,char nEvent,int nStop)
{
    struct termios newtio,oldtio;
    
    if(tcgetattr(fd,&oldtio)!=0){
    
        perror("setup serial 1");
        return -1;
    }
    
    bzero(&newtio,sizeof(newtio));
    newtio.c_cflag |= CLOCAL|CREAD;
    newtio.c_cflag &= ~CSIZE;
    
    switch(nBits){
        
        case 7:
            newtio.c_cflag |= CS7;
            break;
        case 8:
            newtio.c_cflag |= CS8;
            break;
    }
    
    switch(nEvent){
    
        case 'O':
            newtio.c_cflag |= PARENB;
            newtio.c_cflag |= PARODD;
            newtio.c_iflag |= (INPCK|ISTRIP);
            break;
        case 'E':
            newtio.c_cflag |= (INPCK|ISTRIP);
            newtio.c_cflag |= PARENB;
            newtio.c_cflag &= ~PARODD;
            break;
        case 'N':
            newtio.c_cflag &= ~PARENB;
            break;  
    }
    
    switch(nSpeed){
        
        case 2400:
            cfsetispeed(&newtio,B2400);
            cfsetospeed(&newtio,B2400);
            break;
        case 4800:
            cfsetispeed(&newtio,B4800);
            cfsetospeed(&newtio,B4800);
            break;
        case 9600:
            cfsetispeed(&newtio,B9600);
            cfsetospeed(&newtio,B9600);
            break;   
        case 115200:
            cfsetispeed(&newtio,B115200);
            cfsetospeed(&newtio,B115200);
            break;
        case 460800:
            cfsetispeed(&newtio,B460800);
            cfsetospeed(&newtio,B460800);
            break;    
        default:
            cfsetispeed(&newtio,B9600);
            cfsetospeed(&newtio,B9600);
            break;
                             
    }
    
    if(nStop==1){
        newtio.c_cflag &= ~CSTOPB;
    }else if(nStop==2){
    
        newtio.c_cflag |= CSTOPB;
    }
    
    newtio.c_cc[VTIME]=0;
    newtio.c_cc[VMIN]=0;
    tcflush(fd,TCIFLUSH);
    
    if((tcsetattr(fd,TCSANOW,&newtio))!=0){
    
        perror("com set error");
        return -1;
    }
    
    printf("set done!\n\r");
    return 0;
}
