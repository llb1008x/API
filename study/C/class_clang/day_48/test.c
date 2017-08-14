/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com
	> Created Time: 2017年08月14日 星期一 23时12分01秒
 ************************************************************************/

#include <stdio.h>

typedef struct student 
{
    int num;
    char name[20];
    char sex;     

}STU;




int main()
{
    STU boy1,boy2,girl3={104,"lili",'F'};
    printf("input boy1,num,name,sex and score\n");
    
    scanf("%d %s %c",&boy2.num,boy2.name,&boy2.sex);
    boy1=boy2;
    printf("boy1,number=%d,name=%s,sex=%c\n",boy1.num,boy1.name,boy1.sex);
    printf("girl3,number=%d,name=%s,sex=%c\n",girl3.num,girl3.name,girl3.sex);

    return 0;
}
