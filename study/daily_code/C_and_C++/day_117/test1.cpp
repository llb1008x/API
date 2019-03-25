/*
有两个日期，求两个日期之间的天数，如果两个日期是连续的我们规定他们
之间的天数为两天
输入：
有多组数据，每组数据有两行，分别表示两个日期，形式为 YYYYMMDD
输出：
每组数据输出一行，即日期差值


样例输入：
20110412
20110422
样例输出：
11

*/


#include <iostream>
//#include <stdio.h>

using namespace std;

//计算闰年
#define ISYEAP(x) (x)%400!=0 && (x)%4==0 || (x)%400==0?1:0


int main()
{
    printf("%d\n",ISYEAP(2014));  

    return 0;
}
