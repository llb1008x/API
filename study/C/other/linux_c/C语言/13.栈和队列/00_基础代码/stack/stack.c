#include <stdio.h>
#include "stack.h"

static int S[STACK_SIZE]={0};
static int top = 0;

int push(int data)
{
	if (top==STACK_SIZE)
	{
		printf("stack overflow!\n");
		return -1;
	}
	else
	{
		S[top]=data;
		top++;
	}
	return 0;
}

int pop(void)
{
	if (top==0)
	{
		printf("stack empty!\n");
		return -1;
	}
	else
	{
		top--;
		return S[top];
	}
}

int empty(void)
{
	return (top==0)?1:0;
}

int full(void)
{
	return (top==STACK_SIZE)?1:0;
}
