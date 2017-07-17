//进栈算法
#include "stdio.h"
typedef struct node
{
    int data;
    struct node *link;
}NODE;

NODE *top,*head;
void push(int x)
{    
    NODE *temp = (NODE *) malloc(sizeof(NODE));
    temp->data=x;				//将数据插入节点
   // temp->link=NULL;

    temp->link = top;		//调整节点指针
    top = temp;
}

int pop(int *py)
{ 
    NODE *temp=top;

    if (top == head)				//判断是否栈空
    {
        printf("stach empty!");
        return 0;
    }
    else
    {
        *py = top->data;		//将栈中数据读出
        top = top->link;		//调整栈指针
        free (temp);
        return 1;
    }

}


int main()
{ 
    int i,b,y;
    head=top=NULL;

    push(12);         //将11压进栈中
    push(13);         //将11压进栈中
    push(14);         //将11压进栈中

    for( i= 0;i<4;i++)
    {
        
        if ( (b=pop(&y) )!=0)          //将11从栈中弹出
            printf("%d\n",y);
        else
             break;
     }
        getch();
    
}
