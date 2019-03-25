/*************************************************************************
	> File Name: test.c
	> Author:llb 
	> Mail: llb1008x@126.com
	> Created Time: 2019年02月26日 星期二 00时06分34秒
 ************************************************************************/

#include<stdio.h>

int main()
{
    int n,i,j,tmp;
    int buf[100];//保存要排序的数
    
    //循环输入并排序
    while(scanf("%d",&n)!=EOF){
        
        for(i=0;i<n;i++){
            scanf("%d",&buf[i]);
        }
        
        //对输入的数进行排序
        for(i=0;i<n;i++){
            for(j=i;j<n;j++){
            
                if(buf[j]>buf[j+1]){
                    tmp=buf[j+1];
                    buf[j+1]=buf[j];
                    buf[j]=tmp;
                }
            }
        }
        for(i=0;i<n;i++){
            printf("%d\n",buf[i]);
        }
        
        printf("\n");
     }   
    
    return 0;
}
