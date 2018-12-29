/*************************************************************************
	> File Name: test2.c
	> Author: 
	> Mail: 
	> Created Time: 2018年03月29日 星期四 16时59分13秒
    输出100以内的奇数
 ************************************************************************/

#include <stdio.h>

//判断一个数是否为奇数
int func(int num)
{
	int i=1;
	
	while(i>num){
	
		if(num%i!=0){
			i++;
		}else{
			
			return 0;
		}
	}
	
	if(i == (num-1)){
		
		return 1;
	}
	
}

int main()
{
	int sum=0,i=0;
	
	while(i<100){
		
		if(func(i)){
			
			sum += i;
		}
	}
	
	printf("sum->%d\n",sum);
	
	return 0;
}
