/*************************************************************************
    > File Name: link.c
    > Author: llb
    > Mail: llb1008x@126.com
    > Created Time: 2017年09月17日 星期日 13时45分59秒

    实现单向链表的基本操作
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>

//定义数据类型
typedef struct student{

    int num;
    char name[10];
    struct student *next;
    
}STU;


//创建链表
STU *link_create(int num)
{
    STU *head,*pb;
    STU *p_new;
    int i=0;
    
    printf("333333333\n");
    printf("i->%d,num->%d\n",i,num);
    for(i=0;i<num;i++){
        printf("444444444\n");
        printf("i->%d\n",i);
        p_new=(STU *)malloc(sizeof(STU));
        scanf("%d %s",&(p_new->num),p_new->name);
   
        printf("66666666\n");
        if(i==0){
            printf("77777777\n");
            head=pb;
            pb=p_new;
        }else{
            printf("8888888888\n");
            pb->next=p_new;
            pb=p_new;
        }
    }
    printf("555555555\n");

    return head;
}


//遍历链表
//

int main()
{
    int n=0;
    STU *head;

    //scanf("%d",&num);
    scanf("%d",&n);
    printf("1111111111\n");
    head=link_create(n);
    printf("2222222222\n");

    return 0;
}
