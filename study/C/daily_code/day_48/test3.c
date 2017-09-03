/*************************************************************************
	> File Name: test3.c
	> Author: llb
	> Mail: llb1008x@126.com
	> Created Time: 2017年08月14日 星期一 23时45分23秒

结构提的内存分布    
	引入字节对齐可以避免读取效率的下降，结构体内部成员对齐是为了实现用空间换取时间
	
	
	如一个段要从另一个存储单元开始,可以定义:
    unsigned a:1;
    unsigned b:2;
    unsigned :0;
    unsigned c:3;(另一个单元)
由于用了长度为0的位段,其作用是使下一个位段从
下一个存储单元开始存放
将a、b存储在一个存储单元中,c另存在下一个单元

    可以定义无意义位段,如:
    unsigned a: 1;
    unsigned : 2;
    unsigned b: 3;

 ************************************************************************/

#include <stdio.h>

struct data{

#if     1
    char c;
    int i;
    
#else
    unsigned int a:2;
    unsigned int b:6;
    unsigned int c:4;
    unsigned int d:4;
    unsigned int i;

#endif    
};

int main()
{
    struct data stu;
    printf("%ld\n",sizeof(stu));


    return 0;
}
