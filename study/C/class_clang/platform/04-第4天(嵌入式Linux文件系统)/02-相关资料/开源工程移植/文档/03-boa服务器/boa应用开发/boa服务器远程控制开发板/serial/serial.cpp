#include <QtCore>
#include <QQueue>
#include <QApplication>
#include <QtGui>
#include <QWidget>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>

#include "serial.h"

char Serial::g_Msg[5][500];				// 类静态变量必须定义	
QQueue <int> Serial::g_Queue;	


//=============================================================
// 语法格式：	Serial()
// 实现功能：	Serial构造函数，打开并设置串口
// 入口参数：	无
// 出口参数：	无
//=============================================================
Serial::Serial()
{
	g_fd_serial = open("/dev/ttyS0",O_NOCTTY|O_RDWR);
	if(g_fd_serial < 0)
	   qWarning("open /dev/ttyS0 error");
	else
	   qWarning("open /dev/ttyS0 sucess");
	   
	struct termios newtio, oldtio;
	
	tcgetattr(g_fd_serial,&oldtio); /* save current modem settings */
	bzero(&newtio, sizeof(newtio)); 


	newtio.c_cflag = B115200 | CS8 ;
	newtio.c_iflag = IGNPAR | ICRNL; 
	newtio.c_oflag = 0; 
	newtio.c_lflag = ICANON;
	newtio.c_lflag  &= ~(ICANON | ECHO | ECHOE | ISIG);  /*Input raw data*/
	newtio.c_oflag  &= ~OPOST;   /*Output raw data*/

//	 newtio.c_cc[VINTR]    = 0;     /* Ctrl-c */  
//	 newtio.c_cc[VQUIT]    = 0;     /* Ctrl-\ */ 
//	 newtio.c_cc[VERASE]   = 0;     /* del */ 
//	 newtio.c_cc[VKILL]    = 0;     /* @ */ 
//	 newtio.c_cc[VEOF]     = 4;     /* Ctrl-d */ 
//	 newtio.c_cc[VTIME]    = 0;     /* 不使用分割字元组的计时器 */ 
//	 newtio.c_cc[VMIN]     = 1;     /* 在读取到 1 个字元前先停止 */ 
//	 newtio.c_cc[VSWTC]    = 0;     /* '\0' */ 
//	 newtio.c_cc[VSTART]   = 0;     /* Ctrl-q */  
//	 newtio.c_cc[VSTOP]    = 0;     /* Ctrl-s */ 
//	 newtio.c_cc[VSUSP]    = 0;     /* Ctrl-z */ 
//	 newtio.c_cc[VEOL]     = 0;     /* '\0' */ 
//	 newtio.c_cc[VREPRINT] = 0;     /* Ctrl-r */ 
//	 newtio.c_cc[VDISCARD] = 0;     /* Ctrl-u */ 
//	 newtio.c_cc[VWERASE]  = 0;     /* Ctrl-w */ 
//	 newtio.c_cc[VLNEXT]   = 0;     /* Ctrl-v */ 
//	 newtio.c_cc[VEOL2]    = 0;     /* '\0' */ 

 	tcflush(g_fd_serial, TCIFLUSH);
	tcsetattr(g_fd_serial,TCSANOW,&newtio);/*set attrib	  */
											// 定义串口属性结构体变量，其中options用于重新设置串口属性，oldoptions用于原设置，用于恢复到原来的属性									// 打开串口1
	i = 0;
	j = 0;
	read(g_fd_serial,buf,c);
}
