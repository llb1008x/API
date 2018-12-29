/*************************************************************************
	> File Name: demo.c
	> Author:llb 
	> Mail:llb1008x@126.com 
	> Created Time: 2018年08月02日 星期四 23时59分40秒
	
树的存储结构	
	
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>


#define SIZE  10

/*双亲存储结构*/
#if defined(FATHER)
typedef struct 
{
    int data;   //数据域
    int pIdx;   //父节点下标
}TNode;
#endif


/*孩子存储结构*/
#if defined(CHILDREN)
//孩子节点
typedef struct Branch 
{
    int cIdx;
    Branch *next;
}Branch;

//父节点
typedef struct
{
    int data;
    Branch *first;
}TNode;
#endif


/*定义二叉树*/
typedef struct BTNode {

    char data;
    struct BTNode *lchild;
    struct BTNode *rchild;
}BTNode;



/*访问节点函数，打印数值*/
void Visit(BTNode *bt)
{
    printf("bt->data->%d\n",p->data);
}

/*非递归先序遍历*/
void preorderNonrecursion(BTNode *bt)
{
    if(bt!=NULL){
        //初始化一个栈
        BTNode *Stack[SIZE];
        int top=-1;
        
        BTNode *p;
        //根节点入栈
        Stack[++top]=bt;
        while(top!=-1){
            //输出栈顶元素
            p=Stack[top--];
            Visit(p);
            
            //左或右孩子入栈
            if(p->lchild!=NULL){
                Stack[++top]=p->lchild;
            }
            
            if(p->rchild!=NULL){
                Stack[++top]=p->rchild;
            }
        }
    }
}


/*非递归中序遍历*/
void inorderNorecursion(BTNode *bt)
{
    if(bt!=NULL){
        BTNode *Stack[SIZE];
        int top=-1;
        p=bt;
        
        while(top!=-1 || p!=NULL){
            
            //一直遍历左子树，直到最左端，然后弹栈访问
            while(p!=NULL){
            
                Stack[++top]=p;
                p=p->lchild;     
            }
            
            if(top!=-1){
                p=Stack[top--];
                Visit(p);
                p=p->rchild;
            }
 
        }
    }
}

int main()
{

    return 0;
}
