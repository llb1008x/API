/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com
	> Created Time: 2017年09月18日 星期一 22时12分55秒
	一元多项式及其运算
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#define     ElementType     char
#define     MAX             10        

/*******************************************************************************
                定义线性表
*******************************************************************************/                
typedef struct listnode{

    ElementType data[MAX];//存储数据
    ListNode *next;             //指向下一组数据
}ListNode;


/*******************************************************************************
            线性表的操作
*******************************************************************************/


//求表长
int link_length(ListNode *PL)
{
    int i=0;
    
    while(PL){
    
        PL=PL->next;
        i++;
    }

    return i;
}


//按序号查找
ListNode *link_findk(int k,ListNode *PL)
{
    ListNode +p=PL;
    int i=0;
    
    while((p!=NULL)&&(i<k)){
    
        p=p->next;
    }
    
    if(i==k){
    
        return p;
    }else {
    
        return NULL;
    }

}


//按值查找
int link_findv(ElementType x,ListNode *PL)
{

    ListNode *p=PL;
    int i=0;
    
    while((p!=NULL)&&(p->data[i]!=x)){
    
        
    }
}


int main()
{


    return 0;
}
