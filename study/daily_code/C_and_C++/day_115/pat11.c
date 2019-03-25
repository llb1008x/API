/*************************************************************************
	> File Name: pat11.c
	> Author:llb 
	> Mail: llb1008x@126.com
	> Created Time: 2019年02月25日 星期一 22时23分11秒
	6-11 求自定类型元素序列的中位数 （25 分）
本题要求实现一个函数，求N个集合元素A[]的中位数，即序列中第⌊N/2+1⌋大的元素。
其中集合元素的类型为自定义的ElementType。

先要排序
 ************************************************************************/

#include<stdio.h>

#define MAXN 10

typedef float ElementType;

ElementType Median(ElementType A[],int N);

int main()
{
    ElementType A[MAXN];
    int N,i;
    
    scanf("%d",&N);
    for(i=0;i<N;i++)
        scanf("%f",&A[i]);
        
    printf("%.2f\n",Median(A,N));    

    return 0;
}



// 部分正确
ElementType Median(ElementType A[],int N)
{
    int i,j;
    ElementType tmp;
    
    //排序
    for(i=0;i<N;i++){
        for(j=i;j<N;j++){
            if(A[i]>A[j]){
                tmp=A[i];
                A[i]=A[j];
                A[j]=tmp;
            }
        }
    }
    
    for(i=0;i<N;i++)
        printf("A[%d]->%f\n",i,A[i]);
    
    if(N%2==0)
        return (A[N/2-1]+A[N/2+1])/2;
    else
        return A[N/2];    
}


/*  正确答案，用的希尔排序
ElementType Median( ElementType A[], int N)
{
    int i, j, Increment;
    ElementType Tmp;

    //将数组排序
    for ( Increment = N / 2; Increment > 0; Increment /= 2){
        for ( i = Increment; i < N; i++){
            Tmp = A[ i ];
            for (j = i;j >= Increment;j -= Increment ){
                if ( Tmp < A[ j - Increment ])
                    A[ j ] = A[ j - Increment ];
                else
                    break;
            }
            A[ j ] = Tmp;
        }
    }

    return A[ N / 2 ];
}


*/
