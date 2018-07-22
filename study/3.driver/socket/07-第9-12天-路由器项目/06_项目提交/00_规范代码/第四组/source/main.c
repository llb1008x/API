#include <stdio.h>
#include <pthread.h>

#include "get_interface.h"
#include "recv_net_data.h"
#include "firewall_pthread.h"
#include "ip_link.h"
#include "thread_pool.h"

/******************************************************************
功    能:	添加任务到线程池
参	  数:	无
返 回 值:	无
*******************************************************************/
void thread_pool(void)
{	
	pool_t pool;	
	pool_init(&pool, 2);		//初始化一个线程池，其中创建2个线程	

	pool_add_task(&pool, firewall_pthread, NULL);	//添加一个任务	
	pool_add_task(&pool, recv_net_data, NULL);		//添加一个任务	
}

int main(int argc, char *argv[])
{
	read_ip_link();	//过滤ip写入链表

	getinterface();	//获取接口信息
	
	thread_pool();	//创建线程池

	while(1);
	
	return 0;
}