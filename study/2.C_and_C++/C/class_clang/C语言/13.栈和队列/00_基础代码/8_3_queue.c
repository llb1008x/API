//循环队列中加入一个元素的算法：
//设Q[MAX]表示循环队列

#define MAX 7
#include "stdio.h"

int EnQueue(int *Q,int x,int *pf,int *pr)
{
	int front,rear;
	front = *pf;
	rear=*pr;
	if ( (rear+1) % MAX == front ) 	//判断队满
		return 0;			
	else
	{
		Q[rear]=x;					//将数据存入队列
		rear = ( rear+1 ) % MAX;	//队尾指针移位
         *pr=rear;					//保存队尾指针
         return 1;
	}
} 


int DeQueue(int *Q,int *py,int *pf,int *pr)
{
	int front,rear;
	front=*pf;
	rear=*pr;
	if (front==rear)				//判断队空
		return 0;						
	else 
	{
		*py=Q[front];				//将数据读出
		front=(front+1) % MAX;		//队头指针移动
		*pf=front;					//保存队头指针
        return 1;  
	}
}   


int main()
{
	static int s[MAX];
	int rear = 0,front = 0;
	int b=0;
	EnQueue(s,11,&front,&rear);		//入队操作
	EnQueue(s,12,&front,&rear);		//入队操作
	EnQueue(s,13,&front,&rear);		//入队操作
	DeQueue(s,&b,&front,&rear);		//出队操作
	printf("%d\n",b);
	DeQueue(s,&b,&front,&rear);		//出队操作
	printf("%d\n",b);
	DeQueue(s,&b,&front,&rear);		//出队操作
	printf("%d\n",b);
}

