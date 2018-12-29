/*************************************************************************
	> File Name: test.c
	> Author: 
	> Mail: 
	> Created Time: 2018年02月25日 星期日 22时38分24秒
	
题目：猴子第一天摘了若干个桃子，当即吃了一半，还不解馋，又多吃了一个；第二天，
吃剩下的桃子的一半，还不过瘾，又多吃了一个；以后每天都吃前一天剩下的一半多一个，
到第10天想再吃时，只剩下一个桃子了。问第一天共摘了多少个桃子？	

for 循环
//方法二：
//  Created by Neil Wesley on 16/5/23.
//  Copyright © 2016年 Neil Wesley. All rights reserved.

#include<stdio.h>
int main()
{
    int day=9,x1=0,x2=1;
    for(;day>0;day--)
    {
        x1=(x2+1)*2;
        x2=x1;
    }
    printf("The first day of picking peaches number is：%d\n",x1);
}

 ************************************************************************/

#include<stdio.h>


//递归调用
int func(int n)
{
    if(n==10){
        printf("aaaa\n"); 
        return 1;
    }    
    else if(n<10){ 
        printf("bbbb\n");
        return 2*(func(n+1)+1);
    }    
    else {
        printf("error!\n");
        return -1;
    }        
}

int main()
{
    int n,ret;
    
    scanf("%d",&n);
    
    printf("1111\n");
    if(n>10)
        return -1;
    
    printf("2222\n");
    ret=func(n);
    printf("%d\n",ret);

    return 0;
}
