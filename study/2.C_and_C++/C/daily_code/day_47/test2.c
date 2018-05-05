/*************************************************************************
	> File Name: test2.c
	> Author: llb
	> Mail: llb1008x@126.com
	> Created Time: 2017年08月10日 星期四 23时46分34秒
1、
	设计一个计算器，可以进行加、减、乘、除运算。
	要求从键盘输入指令及操作数，如：
	add 15 25		//计算15+25的值，并返回结果
	sub 25 15		//计算25+15的值，并返回结果
	mux 3  5		//计算 3+ 5的值，并返回结果
	div 25 5 		//计算25+ 5的值，并返回结果

提示：
	1.scanf("%s %d %d",name,a,b);
	2. 用字符串比较函数strcmp
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include "my_string.h"

int add(int num1,int num2)
{
   
    return num1+num2;
}


int sub(int num1,int num2)
{
    return num1-num2;
}


int mux(int num1,int num2)
{

    return (num1)*(num2);
}

int div(int num1,int num2)
{

    return (num1)/(num2);
}

int main()
{
//  1.    
    char buf[20]="";
    char opt[10]="";
    int num1,num2,score;
    
//  2. 
    scanf("%s",buf);
    //printf("%s\n",buf);

//  3.
    sscanf(buf,"%s %d %d",opt,num1,num2);
    printf("opt->%s,num1->%d,num2->%d\n",opt,num1,num2);
    
    if(opt=="add"){
    
        score=add(num1,num2);
    }else if(opt=="sub"){
    
        score=sub(num1,num2);
    }else if(opt=="mux"){
    
        score=mux(num1,num2);
    }else if(opt=="div"){
    
        score=div(num1,num2);
    }
    
    printf("score->%d\n",score);
    

    return 0;
}
