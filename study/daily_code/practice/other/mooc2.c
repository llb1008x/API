/*************************************************************************
	> File Name: test.c
	> Author: 
	> Mail: 
	> Created Time: 2019年01月10日 星期四 11时41分51秒
	
	
输出9*9乘法表格
 ************************************************************************/


#include <stdio.h>


int main()
{
    int i,j,result;
    
    for(i=9;i>0;i--){
    
        for(j=1;j<=i;j++){
            
            result=i*j;
            printf("%d*%d=%d ",i,j,result);
        }
        printf("\n");
    }
    
    return 0;
}
