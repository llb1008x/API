/*************************************************************************
	> File Name: test.cpp
	> Author:llb 
	> Mail: llb1008x@126.com
	> Created Time: 2019年02月26日 星期二 23时19分00秒
	
题目描述：
对输入的 n 个数进行排序并输出。
输入：
输入的第一行包括一个整数 n(1<=n<=100)。接下来的一行包括 n 个整数。
输出：
可能有多组测试数据，对于每组数据，将排序后的 n 个整数输出，每个数后
面都有一个空格。每组测试数据的结果占一行。
样例输入：
4
1432
样例输出：
1234	
	
 ************************************************************************/

#include<iostream>
#include<algorithm>

using namespace std;

bool cmp(int x,int y)
{
    return x<y;
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
        sort(buf,buf+n,cmp);
        
        for(int i=0;i<n;i++){
            printf("%d",buf[i]);
        }
        
        printf("\n");
    }

    return 0;
}
