/*************************************************************************
	> File Name: test.c
	> Author: 
	> Mail: 
	> Created Time: 2018年01月10日 星期三 18时41分41秒
 ************************************************************************/

#include <stdio.h>
#include "apue.h"
//#include "ourhdr.h"


int main()
{
	char name[L_tmpnam],line[MAXLINE];
	FILE *fp;
	
	printf("%s\n",tmpnam(NULL));
	
	tmpnam(name);
	
	printf("%s\n",name);
	
	if((fp=tmpfile())==NULL){
		
		err_sys("tmpfile error");
	}
	
	fputs("one line of output\n",fp);
	
	rewind(fp);
	
	if(fgets(line,sizeof(line),fp)==NULL){
	
		err_sys("fgets error");
	}
	
    return 0;
}
