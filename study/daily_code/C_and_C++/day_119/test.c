/*************************************************************************
	> File Name: test.c
	> Author:llb 
	> Mail: llb1008x@126.com
	> Created Time: 2019年03月12日 星期二 00时12分52秒
	
题目描述：
给你 n 个整数，请按从大到小的顺序输出其中前 m 大的数。
输入：
每组测试数据有两行，第一行有两个数 n,m(0<n,m<1000000)，第二行包含 n
个各不相同，且都处于区间[-500000,500000]的整数。
输出：
对每组测试数据按从大到小的顺序输出前 m 大的数。
样例输入：
53
3 -35 92 213 -644
样例输出：
213 92 3
	
 ************************************************************************/

#include<stdio.h>

#define OFFSET  500000 

int Hash[1000001];


int main()
{
    int n,m;
    while(scanf("%d%d",&n,&m)!=EOF){
        
        for(int i=-500000;i<=500000;i++)
            Hash[i+OFFSET]=0;
            
        for(int i=1;i<=n;i++){
            
            int x;
            scanf("%d",&x);
            Hash[x+OFFSET]=1;
        }    
        
        printf("1111111111111\n");
        for(int i=500000;i>=-500000;i--){
            
            if(Hash[i+OFFSET]==1){
                printf("%d",i);
                m--;
                if(m!=0)
                    printf(" ");
                else {
                    printf("\n");
                    break;
                }    
            }
        }
        
        printf("2222222222222\n");
    }
    
    return 0;
}
