/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年08月19日 星期六 09时05分23秒
	
题目：1

	设计一个计算器，可以进行加、减、乘、除运算。
	要求从键盘输入指令及操作数，如：
	add 15 25		//计算15+25的值，并返回结果
	sub 25 15		//计算25-15的值，并返回结果
	mux 3  5		//计算 3* 5的值，并返回结果
	div 25 5 		//计算25/ 5的值，并返回结果	
	
	提示：
	1.scanf("%s %d %d",name,a,b);
	2. 用字符串比较函数strcmp
	
 ************************************************************************/

#include <stdio.h>

int main()
{
    int a,b,ret;
    char name[10]="";
    
    printf("请输入数据:");
    scanf("%s %d %d",name,&a,&b);
    
    if(!strcmp("add",name)){
    
        ret=a+b;
    }else if(!strcmp("sub",name)){
    
        ret=a-b;
    }else if(!strcmp("mux",name)){
    
        ret=a*b;
    }else if(!strcmp("div",name)){
    
        ret=a/b;
    }else {
    
        printf("error\n");
    }

    
    printf("ret->%d\n",ret);

	return 0;
}
