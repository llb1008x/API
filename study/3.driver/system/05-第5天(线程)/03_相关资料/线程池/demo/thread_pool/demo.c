#include <stdio.h>
#include <unistd.h>

#include "thread_pool.h"

void *task_test(void *arg)
{
	printf("\t\tworking on task %d\n", (int)arg);
	sleep(1);			/*��Ϣһ�룬�ӳ������ִ��ʱ��*/
	return NULL;
}

void thread_pool_demo(void)
{
	pool_t pool;
	int i = 0;

	pool_init(&pool, 2);//��ʼ��һ���̳߳أ����д���2���߳�
	sleep(1);
	for(i = 0; i < 5; i++){
		sleep(1);
		pool_add_task(&pool, task_test, (void *)i);//���һ������
	}
	sleep(4);

	pool_uninit(&pool);//ɾ���̳߳�
}

int main (int argc, char *argv[])
{  
	thread_pool_demo();
	return 0;
}