/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com
	> Created Time: 2017年10月08日 星期日 23时12分31秒
 ************************************************************************/

#include <stdio.h>

#define MAXLINE    1000  //输入最大行长度


char pattern[]="ould"; //待查找的模式


//将行保存到s中，并返回该行的长度
int getline(char s[],int lim)
{
    int c,i=0;

    while( (--lim > 0) && (c=getchar())!=EOF && (c!='\n') )
            s[i++]=c;

    if(c=='\n')
        s[i++]=c;

    s[i]='\0';

    return i;
}


//返回t在s中的位置，若未找到则返回-1
int strindex(char s[],char t[])
{
    int i,j,k;

    for(i=0;s[i]!='\0';i++){
        
        for(j=i,k=0;t[k]!='\0' && s[j]==t[k];j++,k++){
                ;
        }

        if(k>0 && t[k]=='\0')
                return i;
    }

    return -1;
}


int main()
{
    char line[MAXLINE];
    int found=0;

    while(getline(line,MAXLINE) > 0 ){
        
        if(strindex(line,pattern) >= 0){

            printf("%s\n",line);
            found++;
        }   
    }
    printf("found->%d\n",found);

    return 0; 
}
