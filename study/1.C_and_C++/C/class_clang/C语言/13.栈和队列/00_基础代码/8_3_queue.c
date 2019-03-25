//ѭ�������м���һ��Ԫ�ص��㷨��
//��Q[MAX]��ʾѭ������

#define MAX 7
#include "stdio.h"

int EnQueue(int *Q,int x,int *pf,int *pr)
{
	int front,rear;
	front = *pf;
	rear=*pr;
	if ( (rear+1) % MAX == front ) 	//�ж϶���
		return 0;			
	else
	{
		Q[rear]=x;					//�����ݴ������
		rear = ( rear+1 ) % MAX;	//��βָ����λ
         *pr=rear;					//�����βָ��
         return 1;
	}
} 


int DeQueue(int *Q,int *py,int *pf,int *pr)
{
	int front,rear;
	front=*pf;
	rear=*pr;
	if (front==rear)				//�ж϶ӿ�
		return 0;						
	else 
	{
		*py=Q[front];				//�����ݶ���
		front=(front+1) % MAX;		//��ͷָ���ƶ�
		*pf=front;					//�����ͷָ��
        return 1;  
	}
}   


int main()
{
	static int s[MAX];
	int rear = 0,front = 0;
	int b=0;
	EnQueue(s,11,&front,&rear);		//��Ӳ���
	EnQueue(s,12,&front,&rear);		//��Ӳ���
	EnQueue(s,13,&front,&rear);		//��Ӳ���
	DeQueue(s,&b,&front,&rear);		//���Ӳ���
	printf("%d\n",b);
	DeQueue(s,&b,&front,&rear);		//���Ӳ���
	printf("%d\n",b);
	DeQueue(s,&b,&front,&rear);		//���Ӳ���
	printf("%d\n",b);
}

