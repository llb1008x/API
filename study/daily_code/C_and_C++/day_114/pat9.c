/*************************************************************************
	> File Name: pat6.c
	> Author:llb 
	> Mail: llb1008x@126.com
	> Created Time: 2019年02月24日 星期日 22时36分37秒
	
6-9 统计个位数字 （15 分）
本题要求实现一个函数，可统计任一整数中某个位数出现的次数。例如-21252中，2出现了3次，
则该函数应该返回3。
 ************************************************************************/

#include <stdio.h>

int Count_Digit ( const int N, const int D );

int main()
{
    int N, D;
	
    scanf("%d %d", &N, &D);
    printf("%d\n", Count_Digit(N, D));

    return 0;
}


int Count_Digit(const int N,const int D)
{
    int n=N;
    int b;
    int p[10]={0};
    
    //n小于0的情况
    if(n<0)
        n = -n;
        
    //N,D都为0的情况
    if(N==0 && D==0)
        return 1;    
        
    if(D<=9 && D>=0){
        while(n){
            b=n%10;
            p[b]++;
            n /=10;
            
            printf("p[%d]->%d\n",b,p[b]);
        } 
        
        return p[D];  
    }else
        return -1;    
           
}
