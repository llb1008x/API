#include <stdio.h>
#include <unistd.h>

#include "thread_pool.h"

void *task_test(void *arg)
{
	printf("\t\tworking on task %d\n", (int)arg);
	sleep(1);			/*休息一秒，延长任务的执行时间*/
	return NULL;
}

void thread_pool_demo(void)
{
	pool_t pool;
	int i = 0;

	pool_init(&pool, 2);//初始化一个线程池，其中创建2个线程
	sleep(1);
	for(i = 0; i < 5; i++){
		sleep(1);
		pool_add_task(&pool, task_test, (void *)i);//添加一个任务
	}
	sleep(4);

	pool_uninit(&pool);//删除线程池
}

int main (int argc, char *argv[])
{  
	thread_pool_demo();
	return 0;
}