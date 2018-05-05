#include <pthread.h>
#include <stdio.h>

#include "ip_link.h"
#include "get_interface.h"
#include "ip_pthread.h"
#include "key_pthread.h"

int main(int argc, char *argv[])
{	
	//获取接口信息
	getinterface();
	
	//创建线程
	pthread_t pth_ip;
	pthread_t pth_key;
	
	pthread_create( &pth_ip, NULL,ip_pthread, NULL );  //处理数据包的线程
	pthread_create( &pth_key,NULL,key_pthread,NULL );  //键盘输入的线程

	pthread_join( pth_ip, NULL );
	pthread_join( pth_key,NULL );

	return 0;
}
