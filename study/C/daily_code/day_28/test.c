/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com
	> Created Time: 2017年06月18日 星期日 20时48分37秒
无符号数和有符号数
************************************************************************/

#include <stdio.h>


int main()
{
    int ret;

    ret= (-2417483647-1 == 2147483648U);
    printf("ret1->%d\n",ret);

    ret= (-2147483647-1 < 2147483647);
    printf("ret2->%d\n",ret);

    ret= (-2147483647-1U < 2147483647);
    printf("ret3->%d\n",ret);

    ret= (-2147483647-1 < -2147483647);
    printf("ret4->%d\n",ret);

    ret= (-2147483647-1U < -2147483647);
    printf("ret5->%d\n",ret);

    return 0;
}
