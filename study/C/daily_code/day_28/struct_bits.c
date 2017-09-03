/*************************************************************************
	> File Name: struct_bits.c
	> Author: llb
	> Mail: llb1008x@126.com
	> Created Time: 2017年06月19日 星期一 23时15分17秒
    位段
************************************************************************/

#include <stdio.h>


struct u0{

    unsigned int leading : 3;
    unsigned int FLAG1 : 1 ;
    unsigned int FLAG2 : 1 ;
    int trailing : 27;
};

void prtBin(unsigned int number)
{
    unsigned mask=1u<<31;
    for(;mask;mask>>=1){

        printf("%d,",number&mask?1:0);
    }
    printf("\n");
}
               

int main()
{
    struct u0 uu;
    uu.leading=2;
    uu.FLAG1=0;
    uu.FLAG2=1;
    uu.trailing=2;
    printf("sizeof(uu)=%lu\n",sizeof(uu));
    prtBin(*(int *)&uu);

   // printf("leading->%p,trailing->%p\n",&(uu.leading),&(uu.trailing));

    return 0;
}
