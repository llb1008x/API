/*************************************************************************
	> File Name: pat6.c
	> Author:llb 
	> Mail: llb1008x@126.com
	> Created Time: 2019年02月24日 星期日 22时36分37秒
	
6-6 求单链表结点的阶乘和 （15 分）
本题要求实现一个函数，求单链表L结点的阶乘和。这里默认所有结点的值非负，且题目保证结果在int范围内。
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>

typedef struct Node *PtrToNode;
struct Node{
    int Data;   //存储节点数据
    PtrToNode next; //只想下一个节点的指针
};

typedef PtrToNode List; // 定义单链表类型

//单链表节点的阶乘和
int FactorialSum(List L);

int main()
{
    int N,i;
    List L,p;
    
    scanf("%d",&N);
    L=NULL;
    //创建单链表
    for(i=0;i<N;i++){
        p=(List)malloc(sizeof(struct Node));
        scanf("%d",&p->Data);
        p->next=L;
        L=p;
    }
    
    printf("%d\n", FactorialSum(L));
    
    return 0;
}

int FactorialSum(List L)
{
    //PtrToNode p=L;
    int sum=0,ret,i;
    
    while(L!=NULL){
        
        ret=1;
        for(i=1;i<=L->Data;i++){
                ret = ret*i;
        }        
                
        sum = sum + ret;        
        L=L->next;
    }
    
    return sum;
}
