/*************************************************************************
	> File Name: printN.c
	> Author: llb
	> Mail: llb1008x@126.com
	> Created Time: 2017年09月03日 星期日 09时10分31秒
 ************************************************************************/

#include <stdio.h>

#if     1
int printN(int N)
{
    if(N){
        printf("%d\n",N);
        printN(N-1);
    }
    
    return 0;
}

#else
int printN(int N)
{
    int i;

    for(i=0;i<N;i++){

        printf("%d\n",N);
    }

    return 0;
}
#endif

int main()
{
    int N;
    scanf("%d",&N);

    printN(N);

    return 0;
}

