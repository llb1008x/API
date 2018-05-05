/*************************************************************************
	> File Name: test2.c
	> Author: llb
	> Mail: llb1008x@126.com
	> Created Time: 2017年08月14日 星期一 23时37分09秒
	
	定义一个结构体数组,求学生平均成绩

 ************************************************************************/

#include <stdio.h>

typedef struct student 
{
    int num;
    char name[20];
    float score;

}STU;

int main()
{
    
    STU edu[3]={
    
            {101,"lucy",78},
            {102,"bob",59.5},
            {103,"tom",85}
    };
    int i;
    float aver,sum;
    
    for(i=0;i<3;i++){
    
        sum += edu[i].score;
    } 
   
    aver=sum/3;
    
    printf("aver->%lf\n",aver);


    return 0;
}
