/*
1006 换个格式输出整数 （15 分)
让我们用字母 B 来表示“百”、字母 S 表示“十”，用 12...n 来表示不为零的个位数字 n（<10），换个格式来输出任一个不超过 3 位的正整数。例如 234 应该被输出为 BBSSS1234，因为它有 2 个“百”、3 个“十”、以及个位的 4。

输入格式：
每个测试输入包含 1 个测试用例，给出正整数 n（<1000）。

输出格式：
每个测试用例的输出占一行，用规定的格式输出 n。

输入样例 1：
234
输出样例 1：
BBSSS1234
输入样例 2：
23
输出样例 2：
SS123

最后输出的是个位从一到个位一共几个


*/

#include <iostream>
#include <vector>
#include <cstring>

using namespace std;

int main()
{
    int n,m,p,k;
    //vector<char>V;
    //char str[30];
    //memset(str,0,30);
    
    cin>>n;
    
    //输入判断
    if(n>=1000 || n<=0)
        return -1;
        
    m=n/100;
    if(m!=0){
        int i=m;
        while(i!=0){
            printf("B");
            i--;
        }
    }
    
    p=n%100/10;
    if(p!=0){
        int i=p;
        while(i!=0){
            printf("S");
            i--;
        }
    }
    
    k=n%10;
    for(int i=1;i<=k;i++)
        printf("%d",i);
        
    return 0;
}

