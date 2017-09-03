/******************************************************
*C语言练习            
*2017.4.10
******************************************************/
#include <stdio.h>

#define YEAR    365

/**********************************************
将年龄转化为天数,先不考虑平年润年
输入：输入的年龄
输出：转化成的天数
***********************************************/
int Old_to_Day(int old)
{
    int days=0;
    days=old*YEAR;

    return days;
}

/*********************************************
简单的计算
输入：输入要计算的数,参数的多少次方
输出：计算的结果
*********************************************/
int power(int num,int n)
{
    

    return num;
}


int main()
{
    printf("hello world\n");

    int num=0,n=0;
    printf("请输入要计算的参数：\n");
    scanf("%d %d",&num,&n);

    printf("计算结果是：%d\n",power(num,n));
/*
    int old,days;

    printf("请输入你年龄：\n");

    scanf("%d",&old);
    days=Old_to_Day(old);
    printf("你活了 %d 天\n",days);
*/
    return 0;

}