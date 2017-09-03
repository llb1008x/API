/*************************************************************************
	> File Name: subseqsum.c
	> Author: llb
	> Mail: llb1008x@126.com
	> Created Time: 2017年09月03日 星期日 15时10分43秒

 最大子列和问题   
 ************************************************************************/

#include <stdio.h>
#include <string.h>

#define MAX_SIZE    10

//algo_1
int subseqsum1(int a[],int N)
{
    int i,j,k;
    int sum=0,this_sum=0;

    //子列的最左端
    for(i=0;i<N;i++){
        //子列的最右端
        for(j=i;j<N;j++){
            this_sum=0;
            //计算子列从左到右的子列和
            for(k=i;k<j;k++){
    
                this_sum += a[k];
                printf("k->%d,sub sum->%d\n",k,this_sum);
            }
            
            if(this_sum > sum)
                 sum=this_sum;
        }
    }

    return sum;
}


//algo_2
int subseqsum2(int a[],int N)
{
    int i,j;
    int this_sum,sum;
    
    //子列的最左端
    for(i=0;i<N;i++){

        this_sum=0;
        //子列的最右端
        for(j=i;j<N;j++){
            
            this_sum += a[j];
            printf("j->%d,this_sum->%d\n",j,this_sum);
        
        if(this_sum>sum)
            sum=this_sum;
        }

    }

    return sum;
}


//algo_3
#if defined(TEST)


//在C语言标准中规定：条件表达式（三目运算符）的结合性是右结合，即自右向左结合
int Max3( int A, int B, int C )
{ /* 返回3个整数中的最大值 */
    return A > B ? (A > C ? A : C) : (B > C ? B : C);
}
 
int DivideAndConquer( int List[], int left, int right )
{ /* 分治法求List[left]到List[right]的最大子列和 */
    int MaxLeftSum, MaxRightSum; /* 存放左右子问题的解 */
    int MaxLeftBorderSum, MaxRightBorderSum; /*存放跨分界线的结果*/
 
    int LeftBorderSum, RightBorderSum;
    int center, i;
 
    if( left == right )  { /* 递归的终止条件，子列只有1个数字 */
        if( List[left] > 0 )  
            return List[left];
        else
            return 0;
    }
 
    /* 下面是"分"的过程 */
    center = ( left + right ) / 2; /* 找到中分点 */
    /* 递归求得两边子列的最大和 */
    MaxLeftSum = DivideAndConquer( List, left, center );
    MaxRightSum = DivideAndConquer( List, center+1, right );
 
    /* 下面求跨分界线的最大子列和 */
    MaxLeftBorderSum = 0; 
    LeftBorderSum = 0;
    for( i=center; i>=left; i-- ) { /* 从中线向左扫描 */
        LeftBorderSum += List[i];
        if( LeftBorderSum > MaxLeftBorderSum )
            MaxLeftBorderSum = LeftBorderSum;
    } /* 左边扫描结束 */
 
    MaxRightBorderSum = 0; 
    RightBorderSum = 0;
    for( i=center+1; i<=right; i++ ) { /* 从中线向右扫描 */
        RightBorderSum += List[i];
        if( RightBorderSum > MaxRightBorderSum )
            MaxRightBorderSum = RightBorderSum;
    } /* 右边扫描结束 */
 
    /* 下面返回"治"的结果 */
    return Max3( MaxLeftSum, MaxRightSum, MaxLeftBorderSum + MaxRightBorderSum );
}
 
int MaxSubseqSum3( int List[], int N )
{ /* 保持与前2种算法相同的函数接口 */
    return DivideAndConquer( List, 0, N-1 );
}

#enif

//algo_4
int subseqsum4(int a[],int N)
{
    int this_sum,sum;
    int i;

    this_sum=sum=0;
    for(i=0;i<N;i++){
        //向右累加
        this_sum += a[i];
        printf("this_sum->%d\n",this_sum);

        if(this_sum>sum){
            sum=this_sum;
        }else if(this_sum<0){
            
            this_sum=0;
        }
     
    }   

    return sum;
}



int main()
{
    int a[]={1,5,3,4,7,2,-1,5,7,-2};
    int size=sizeof(a)/sizeof(int);
    int sum;

    sum=subseqsum4(a,size);
    printf("main sum->%d\n",sum);

    return 0;
}
