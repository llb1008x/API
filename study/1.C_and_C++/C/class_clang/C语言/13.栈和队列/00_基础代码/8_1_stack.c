//��ջ�㷨
#include "stdio.h"
#define stacksize 100			/*����stacksizeΪ����100 */

int push(int s[],int x,int *ptop)
{    
	int top; 
    top=*ptop;	//ptop��ָ�������*ptop���ʵ��ջ��ָ��
	if ( top==stacksize )		//ջ��
     {
		printf("overflow\n");
		return 0;
	}
	else 
	{ 
		s[top]=x; 
		top++;
		*ptop=top;		//ʵ��ջ��ָ���1�����ص����ú�����
		return 1;   
	}
}

int pop(int s[],int *py,int *ptop)
{ 
	int top;
	top=*ptop;					//ptop��ָ�������*ptop���ʵ��ջ��ָ��
	if (top==0)					//ջ��	
	{
	   printf("stack empty\n");
	   return 0;
	}
   else 
   {	
	   --top;
	   *py=s[top];				//ʵ��ջ��ָ���1�����ص����ú�����
       *ptop=top;    
		return 1; 
   }
}


int main()
{ 
	static int s[stacksize];
    int top=0,result,y;
	int i;

    result=push(s,11,&top);	//��11ѹ��ջ��
	result=push(s,22,&top);	//��22ѹ��ջ��
	result=push(s,33,&top);	//��33ѹ��ջ��
    printf("top=%d\n",top);

	for (i=0;i<3;i++)
	{
		if ( (result=pop(s,&y,&top)) == 1 ) 
			printf("top=%d,y=%d\n",top,y);
	}
}

