/*************************************************************************
	> File Name: producer_consumer.c
	> Author:llb 
	> Mail: llb1008x@126.com
	> Created Time: 2019年02月12日 星期二 17时16分35秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

#define msleep(x)  usleep(x*1000)

#define PRODUCT_SPEED  200 //生产者速度
#define CONSUM_SPEED   450 //消费者速度
#define INIT_NUM       3   //仓库原有产品数
#define TOTAL_NUM      10  //仓库容量

sem_t p_sem,c_sem,sh_sem;
int num=INIT_NUM;       //仓库产品数量

//生产延时
void produce_delay(void)
{
    msleep(PRODUCT_SPEED*10);
}

//消费延时
void consumer_delay(void)
{
    msleep(CONSUM_SPEED*10);
}

//添加产品到仓库
int add_to_lib(void)
{
    sem_wait(&sh_sem);
    num++;
    msleep(300);
    sem_post(&sh_sem);

    return num;
}

//从仓库中取出产品
int sub_from_lib(void)
{
    sem_wait(&sh_sem);
    num--;
    msleep(300);
    sem_post(&sh_sem);

    return num;
}


//生产者线程
void  *producer(void *arg)
{
    int num=0;

    while(1){

        sem_wait(&p_sem);
        produce_delay();
        num=add_to_lib();
        printf("push into! total_num=%d\n",num);
        sem_post(&c_sem);
    }
}


//消费者线程
void *consumer(void *arg)
{
    int num=0;

    while(1){

        sem_wait(&c_sem);
        num=sub_from_lib();
        printf("pop out! total_num=%d\n",num);
        sem_post(&p_sem);
        consumer_delay();
    }
}


int main()
{
    pthread_t tid1,tid2;

    sem_init(&p_sem,0,TOTAL_NUM-INIT_NUM);
    sem_init(&c_sem,0,INIT_NUM);
    sem_init(&sh_sem,0,1);

    pthread_create(&tid1,NULL,producer,NULL);
    pthread_create(&tid2,NULL,consumer,NULL);

    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);

    return 0;
}
