#include <stdio.h>
#include "queue.h"

static int Q[QUEUE_SIZE]={0};
static int front = 0;
static int rear = 0;


int enqueue(int data)
{
	if ((rear+1)%QUEUE_SIZE==front)
	{
		printf("queue is full!\n");
		return 0;
	}
	Q[rear] = data;
	rear = (rear+1)%QUEUE_SIZE;
	return 1;
}
int dequeue(void)
{
	int temp = 0;
	if (rear==front)
	{
		printf("queue is empty!\n");
		return -1;
	}
	temp = Q[front];
	front = (front+1)%QUEUE_SIZE;
	return temp;
}