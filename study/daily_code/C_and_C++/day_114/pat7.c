/*************************************************************************
	> File Name: pat6.c
	> Author:llb 
	> Mail: llb1008x@126.com
	> Created Time: 2019年02月24日 星期日 22时36分37秒
	
6-7 统计某类完全平方数 （20 分）
本题要求实现一个函数，判断任一给定整数N是否满足条件：它是完全平方数，又至少有两位数字相同，如144、676等。



 ************************************************************************/

#include <stdio.h>
#include <math.h>

int IsTheNumber ( const int N );

int main()
{
    int n1, n2, i, cnt;
	
    scanf("%d %d", &n1, &n2);
    cnt = 0;
    for ( i=n1; i<=n2; i++ ) {
        if ( IsTheNumber(i) )
            cnt++;
    }
    printf("cnt = %d\n", cnt);

    return 0;
}


/*
int IsTheNumber(const int N)
{
    int i;
    
    for(i=0;i<N;i++){
        if(N == i*i){
            printf("i->%d\n",i);
            break;
        }
         
        if(N < i*i)
            return 0;
    }
    
    
    //N是多大还不知道
    int a,b,c;
    if(N>=100){
        a=N/100;
        b=N%100/10;
        c=N%10;
    }
    
    printf("a->%d,b->%d,c->%d\n",a,b,c);
    
    if(a!=b && a!=c && b!=c)
        return 0;
    else 
        return 1;    
    
}
*/


/*这个处理的很好，需要学习*/
int IsTheNumber ( const int N )
{
  int n=N;
  int b;
  int p[10]={0};
  int m=sqrt(n);
  if(m*m==n){
    while(n){
      //把n的每一位保存在一个数组中，如果某个数在其他位上也有，计数就大于1，是完全平方数
      b=n%10;
      p[b]++;
      n/=10;
      
      printf("p[%d]->%d,n->%d\n",b,p[b],n);
    }
    for(int i=0;i<=9;i++){
      if(p[i]>1)
        return 1;
    }
  }
  return 0;
}



