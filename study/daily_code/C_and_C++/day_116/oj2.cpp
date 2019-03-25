/*************************************************************************
	> File Name: oj2.cpp
	> Author:llb 
	> Mail: llb1008x@126.com
	> Created Time: 2019年02月26日 星期二 23时39分40秒
	
题目描述：
有 N 个学生的数据，将学生数据按成绩高低排序，如果成绩相同则按姓名
字符的字母序排序，如果姓名的字母序也相同则按照学生的年龄排序，并输出 N
个学生排序后的信息。
输入：
测试数据有多组，每组输入第一行有一个整数 N（N<=1000），接下来的 N
行包括 N 个学生的数据。每个学生的数据包括姓名（长度不超过 100 的字符串）       、
年龄（整形数）、成绩（小于等于 100 的正数）。
输出：
将学生信息按成绩进行排序，成绩相同的则按姓名的字母序进行排序。然后
输出学生信息，按照如下格式：姓名 年龄 成绩
样例输入：
3
abc 20 99
bcd 19 97
bed 20 97
样例输出：
bcd 19 97
bed 20 97
abc 20 99
提示：
学生姓名的字母序区分字母的大小写，如 A 要比 a 的字母序靠前(因为 A 的
ASC 码比 a 的 ASC 码要小)。	
	
 ************************************************************************/

#include<iostream>
#include<algorithm>
#include<string.h>

using namespace std;

struct student{

    char name[101];
    int age;
    int score;
}buf[1000];


//重载比较函数
bool cmp(student a,student b)
{
    if(a.score!=b.score)
        return a.score<b.score;   //分数不相同，返回分数低的
     
     int tmp=strcmp(a.name,b.name);
     if(tmp!=0)     //分数相同自序小的在前
        return tmp<0;
      else 
        return a.age<b.age;      //名字相同则年龄小的在前     
}

int main()
{
    int n;
    
    while(scanf("%d",&n)!=EOF){
        for(int i=0;i<n;i++)
            scanf("%s%d%d",buf[i].name,&buf[i].age,&buf[i].score);

        sort(buf,buf+n,cmp);
        printf("******************************************\n");
        for(int i=0;i<n;i++){
            printf("%s %d %d\n",buf[i].name,buf[i].age,buf[i].score);
    }

    
    }

    return 0;
}
