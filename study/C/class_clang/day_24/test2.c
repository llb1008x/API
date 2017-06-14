/*************************************************************************
	> File Name: test2.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年06月14日 星期三 08时46分00秒
 ************************************************************************/

#include<stdio.h>

int main()
{
	FILE* fp=fopen("text.txt","r");
	int num;

	if(fp!=NULL){
	
		fscanf(fp,"%d",&num);
		printf("%d\n",num);
		fclose(fp);
	}else{
		
		return -1;
	}
	
	return 0;
}
