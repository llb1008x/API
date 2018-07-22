/*************************************************************************
	> File Name: test.c
	> Author:llb 
	> Mail:llb1008x@126.com 
	> Created Time: 2018年07月01日 星期日 23时24分44秒

顺序表的基本操作
    1.查找
    2.插入
************************************************************************/

#include <stdio.h>

#define MAXSIZE   15

typedef struct {
    int data[MAXSIZE];
    int length;
}Sqlist;

/*
* fun:查找顺序表中的元素
* param:L 顺序表,x 待找的元素
* return:元素的位置
* */
int findElem(Sqlist L,int x)
{
    int i;
    
    for(i=0;i<L.length;i++){
        if(L.data[i]>x)
            return i;
    }

    return i;
}

/*
*fun:顺序表中插入元素
*param:L,顺序表，x待插入的元素
*return ，void
* */
void insertElem(Sqlist *L,int x)
{
    int i,k;
    
    k=findElem(*L,x);
    printf("k->%d\n",k);

    for(i=L->length-1;i>=k;i--){
        /*元素向右移动*/
        L->data[i+1]=L->data[i];
    }

    L->data[k]=x;
    L->length+1;
    
    i=0;
    while(L->data[i]!='\0'){
        
        printf("L->data[%d]->%d\n",i,L->data[i]);
        i++;
    }
}


int main()
{
    int i,num1,num2;
    Sqlist L={{1,3,6,7,8,9,11,13},MAXSIZE};
    
    scanf("%d %d",&num1,&num2);

    i=findElem(L,num1);
    printf("i->%d\n",i);

    insertElem(&L,num2);

    return 0;
}

