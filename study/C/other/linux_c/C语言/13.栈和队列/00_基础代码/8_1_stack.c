//进栈算法
#include "stdio.h"
#define stacksize 100			/*定义stacksize为常数100 */

int push(int s[],int x,int *ptop)
{    
	int top; 
    top=*ptop;	//ptop是指针变量；*ptop获得实际栈顶指针
	if ( top==stacksize )		//栈满
     {
		printf("overflow\n");
		return 0;
	}
	else 
	{ 
		s[top]=x; 
		top++;
		*ptop=top;		//实际栈顶指针加1，返回到调用函数处
		return 1;   
	}
}

int pop(int s[],int *py,int *ptop)
{ 
	int top;
	top=*ptop;					//ptop是指针变量；*ptop获得实际栈顶指针
	if (top==0)					//栈空	
	{
	   printf("stack empty\n");
	   return 0;
	}
   else 
   {	
	   --top;
	   *py=s[top];				//实际栈顶指针减1，返回到调用函数处
       *ptop=top;    
		return 1; 
   }
}


int main()
{ 
	static int s[stacksize];
    int top=0,result,y;
	int i;

    result=push(s,11,&top);	//将11压进栈中
	result=push(s,22,&top);	//将22压进栈中
	result=push(s,33,&top);	//将33压进栈中
    printf("top=%d\n",top);

	for (i=0;i<3;i++)
	{
		if ( (result=pop(s,&y,&top)) == 1 ) 
			printf("top=%d,y=%d\n",top,y);
	}
}

