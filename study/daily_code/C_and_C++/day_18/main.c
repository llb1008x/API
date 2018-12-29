/*************************************************************************
	> File Name: main.c
	> Author: llb
	> Mail: llb1008x@126.com
    > Created Time: 2017年05月20日 星期六 16时43分11秒
    练习使用分函数，分文件
    功能：对一个学生的成绩进行评定
 ************************************************************************/

#include <stdio.h>
#include "score.h"

int main()
{
    int score=0;
    char* rank;
  
    printf("请输入学生成绩:\n");
    scanf("%d",&score);

    rank=fun(score);
    printf("rank->%s\n",rank);

    return 0;
}
