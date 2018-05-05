/*************************************************************************
	> File Name: test3.c
	> Author: 
	> Mail: 
	> Created Time: 2018年02月28日 星期三 15时41分12秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>

int my_strlen(char *str)
{
    int i=0;

    if(str == NULL)
        return 0;

    while( str[i] != '\0'){

        i++;
    }
    
    return i;
}

char *my_strcpy(char *dest,char *str)
{
    int i=0;
    dest=(char *)malloc(sizeof(str)+1);

    if(sizeof(dest) < sizeof(str)){

        printf("error!\n");
        return NULL;
    }

    while(str[i] != '\0'){
            
        dest[i]=str[i];
        i++;
        printf("i->%d\n",i);
    }

    return dest;
}


char *mystrcat(char *dst,char *src)
{
	char *tmp=dst;
	int i=0,j=0;

	while( dst[i]!='\0'){
		
		printf("i->%d\n",i);
		i++;
	}
	
	while(dst[i]=src[j]){
	
		printf("i->%d,j->%d\n",i,j);
		i++;
		j++;
	}
	dst[i]='\0';

	return dst;
}


int main()
{
	char str1[20]="hello";
	char str2[10]="world";
	
	char *dest=mystrcat(str1,str2);
	printf("dest->%s\n",dest);

    return 0;
}

