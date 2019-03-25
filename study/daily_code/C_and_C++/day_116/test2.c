/*************************************************************************
	> File Name: test.cpp
	> Author:llb 
	> Mail: llb1008x@126.com
	> Created Time: 2019年02月26日 星期二 23时19分00秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>

int cmp(int x,int y)
{
    return x>y;
}



int main()
{
    int n;
    int buf[100];
    
    while(scanf("%d",&n)!=EOF){
        
        for(int i=0;i<n;i++){
            scanf("%d",&buf[i]);
        }
        
        //sort(buf,buf+n,cmp);
        qsort(buf,n-1,sizeof(int *),cmp);
        
        for(int i=0;i<n;i++){
            printf("%d",buf[i]);
        }
        
        printf("\n");
    }

    return 0;
}
