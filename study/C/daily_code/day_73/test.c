/*************************************************************************
	> File Name: test.c
	> Author: 
	> Mail: 
	> Created Time: 2017年11月17日 星期五 15时42分32秒
	
	
	
Ah Love! could you and I with Fate conspire
To grasp this sorry Scheme of Things entire,
Would not we shatter it to bits -- and then
Re-mould it nearer to the Heart's Desire!
 ************************************************************************/
#include <stdio.h>

#define MAXLINE 	1000

int mygetline(char line[],int max);

int strindex(char source[],char search[]);


char pattern[]="ould";


int main()
{
	char line[MAXLINE];
	int found=0;
	
	while(mygetline(line,MAXLINE)>0){
		
		if(strindex(line,pattern)>=0){
			printf("%s",line);
			found++;
		}
	}
	
	return found;
}


int mygetline(char s[],int lim)
{
	int c,i=0;
	
	while(--lim>0 && (c=getchar())!=EOF && c!='\n'){
	
		s[i++]=c;
	}

	if(c=='\n'){
		
		s[i++]=c;
	}
	
	s[i++]='\0';

	return i;
}


int strindex(char s[],char t[])
{
	int i,j,k;
	
	for(i=0;s[i]!='\0';i++){
		for(j=i,k=0;t[k]!='\0' && s[j]==t[k];j++,k++){
						printf("cccc\n");
		}
						
			
		if(k>0 && t[k]=='\0'){
			return i;
		}
		
	}	
	
	return -1;
}
