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

//in输入,out 输出,count表示产品变化
int in=0,out=0,count=0;
int num=INIT_NUM;
int n=TOTAL_NUM;
int nextp,nextc;

//缓冲池
int buffer[TOTAL_NUM];

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
    num++;
    msleep(300);

    return num;
}

//从仓库中取出产品
int sub_from_lib(void)
{

    num--;
    msleep(300);

    return num;
}


//生产者线程
void  *producer(void *arg)
{
    while(1){

        produce_delay();
        //生产一件产品
        nextp=add_to_lib();
        while(count==n)
                    ;
        //添加到缓冲池中            
        buffer[in]=nextp;
        in=(in+1)%n;
        count++;            
        
        printf("push into! total_num=%d\n",count);
    }
}


//消费者线程
void *consumer(void *arg)
{
    while(1){
        
        while(count==0)
                     ;
        nextc=buffer[out];
        out=(out+1)%n;
        count--;
                    
        count=sub_from_lib();
        printf("pop out! total_num=%d\n",count);
        
        consumer_delay();
    }
}


int main()
{
    pthread_t tid1,tid2;
    
    pthread_create(&tid1,NULL,producer,NULL);
    pthread_create(&tid2,NULL,consumer,NULL);

    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);

    return 0;
}
