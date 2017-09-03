/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年08月02日 星期三 19时42分53秒
 ************************************************************************/

#include<stdio.h>

#define BIG  0

int main()
{

	char str[20]="C language";
	char C;
	int i=0;
	
	//printf("11111111111\n");
	while( (C=str[i++])!='\0'){
		//printf("222222222222\n");
		#if BIG
		 	if(C>='a' && C<='z')
		 		C=C-32;
		#else 
			if(C>='A' && C<='Z')
				C=C+32;
		#endif
		printf("%c",C);		 			
	}

	return 0;
}
