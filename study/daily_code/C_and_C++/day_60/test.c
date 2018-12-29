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
    int last;             //数据的最后一个位置
}ListNode;

/*******************************************************************************
            线性表的操作
*******************************************************************************/

//创建表
ListNode *list_create()
{
    ListNode *PL;
    
    PL=(ListNode *)malloc(sizeof(ListNode));
    PL->last=-1;
    
    return PL;
}

//释放表
void list_free(ListNode *PL)
{
    free(PL);
    PL=NULL;
    //PL->last=-1;
}

//查找表中相应元素的位置
int list_find(ElementType x,ListNode *PL)
{
    int i;
    int ret;
    
    while((i<PL->last)&&(PL->data[i]!=x))
           i++;
           
    if(i>(PL->last))
        ret=-1;
    else
        ret=i;
     
    return ret;                
}


//插入数据到表中
void list_insert(ElementType x, int k,ListNode *PL)
{
    int ret,i;
    //先判断表的情况
    if((PL->last)>=MAx-1){
    
        printf("it's over\n");
        return ;
    }
    
    //判断插入的情况
    if((k<1)||(k>(PL->last+2))){
    
        printf("it's unsafty\n");
        return ;
    }
    
    //顺序移动元素的位置
    for(i=PL->last;i>=k-1;i--){
        
        PL->data[i+1]=PL->data[i];
    }

    PL->data[i-1]=x;
    PL->last++;
        
}


//删除某个位置上的数据
void list_delete(int k,ListNode *PL)
{

    int i;
    
    //判断删除的数据位置是否合法
    if(k<1||(k>PL->last+1)){
    
        printf("not exist this statution\n");
        return ;  
    }
    
    //数据位置移位
    for(i=k;i<(PL->last);i++){
    
        PL->data[i-1]=PL->data[i];
    }
    
    PL->last--;
}




int main()
{
    ListNode *PL;
    
    printf("11111111\n");
    PL=list_create();
    printf("22222222\n");
    list_free(PL);
    printf("33333333\n");

    return 0;
}
