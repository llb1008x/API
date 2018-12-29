/*************************************************************************
	> File Name: test4.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年08月07日 星期一 20时41分45秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>

int main()
{
	char str1[]="adc*fvcv*!abcy!hgbdfg*casdert";
	char *str2="*!";
	char *result[10];
	int num=0,i;
	
	result[num]=strtok(str1,str2);
	while(result[num]!=NULL){
	
		num++;
		result[num]=strtok(NULL,str2);
		printf("result[%d]=%s,str1->%s\n",i,result[i],str1);
	}
	
	/*
	for(i=0;i<num;i++){
		
		printf("result[%d]=%s\n",i,result[i]);
	}
	*/


	return 0;
}
