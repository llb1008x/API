//��ջ�㷨
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
    temp->data=x;				//�����ݲ���ڵ�
   // temp->link=NULL;

    temp->link = top;		//�����ڵ�ָ��
    top = temp;
}

int pop(int *py)
{ 
    NODE *temp=top;

    if (top == head)				//�ж��Ƿ�ջ��
    {
        printf("stach empty!");
        return 0;
    }
    else
    {
        *py = top->data;		//��ջ�����ݶ���
        top = top->link;		//����ջָ��
        free (temp);
        return 1;
    }

}


int main()
{ 
    int i,b,y;
    head=top=NULL;

    push(12);         //��11ѹ��ջ��
    push(13);         //��11ѹ��ջ��
    push(14);         //��11ѹ��ջ��

    for( i= 0;i<4;i++)
    {
        
        if ( (b=pop(&y) )!=0)          //��11��ջ�е���
            printf("%d\n",y);
        else
             break;
     }
        getch();
    
}
