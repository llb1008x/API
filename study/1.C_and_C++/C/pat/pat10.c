/*************************************************************************
	> File Name: pat6.c
	> Author:llb 
	> Mail: llb1008x@126.com
	> Created Time: 2019年02月24日 星期日 22时36分37秒
	
6-10 阶乘计算升级版 （20 分）
本题要求实现一个打印非负整数阶乘的函数。
 ************************************************************************/

#include <stdio.h>

void Print_Factorial ( const int N );

int main()
{
    int N;
	
    scanf("%d", &N);
    Print_Factorial(N);
    return 0;
}



void Print_Factorial(const int N)
{
    int i;
    long ret=1;

    if(N>1000 || N<0){
        
        printf("Invalid input\n");
    }
    
    
    //如果数值太大，阶乘的范围超过了long的精度，表示不了
    for(i=1;i<=N;i++){
        ret = ret*i;
    }
    
    printf("%ld\n",ret);
}


/* 正确版本
void Print_Factorial ( const int N ){
	long i,s=1;
	if(N>=0&&N<=12){
		for(i=2 ;i<=N ;i++){
			s *= i;
		}
		printf("%ld\n",s);
	}
	//这里是正确的处理方法，虽然有点麻烦
	else if(N>12&&N<=1000){
		int num[3000] = {0};
		num[0] = 1;
		int k=1;  //位数
		int n=0;  //进位
		int temp;
		for(int i=2 ;i<=N ;i++){
			for(int j=0;j<k;j++){
				temp = num[j]*i+n;  //每一位相乘 再+进位
				num[j] = temp%10;	//更新每一位的数字
				n = temp/10;		//判断能否进位
			}	
			while(n!=0){  //如果可以进位
				num[k] = n%10;  //新增一位
				n /=10;  //继续判断能否进位
				k++;
			}
		}
		for(int x=k-1;x>=0;x--){		//输出数字
			printf("%d",num[x]);
		}
	}
	else{
		printf("%s\n","Invalid input");
	}
}
*/
