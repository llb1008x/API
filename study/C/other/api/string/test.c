/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年08月07日 星期一 20时52分10秒
	
	自定义字符串操作函数
	version 1.0  2017.8.7
 ************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



//计算字符串的长度
int mystrlen(char *str)
{
    int i=0;
    
    if(str==NULL){
    
    	return -1;
    }

#if  0   
    while(str[i]!='\0'){
        
        i++;
        printf("i->%d\n",i);
    }
    
#else 
    //想想为什么以这种方式是错的，指针方式应该怎么写
    // str!=NULL. *str !='\0'
    while(*str != '\0'){
    
        str++;
        i++;
    }
    
#endif   

    return i;
}


//比较两个字符串的大小
int mystrcmp(char *str1,char *str2)
{
    int ret=0,i=0;
    
    if( (str1==NULL)&&(str2==NULL) ){
    
    	return -1;
    }
    
    while( (*str1!='\0')||(*str2!='\0') ){
    
        printf("i->%d\n",i);
        if(*str1 != *str2){
        
            ret= *str1-*str2;
            break;
        }
        
        str1++;
        str2++;
        i++;
    }
    
    return ret;
}

//比较两个字符串的前n个字节
int mystrncmp(char *str1,char *str2,int n)
{
	int ret=0,i=0;
	
	if( ((str1==NULL)&&(str2==NULL))||(n<0) ){
	
		return -1;
	}
	
	while( ((*str1!='\0')||(*str2!='\0'))&&(i<n-1) ){
	
			printf("i->%d\n",i);
			if(*str1 != *str2){
			
				ret = *str1-*str2;
				break;
			}
			str1++;
			str2++;
			i++;
	}
		
	return ret;
}



//字符串拷贝函数
char *mystrcpy(char *dst,char *src)
{
	char *tmp=dst;
	int i=0;
	
	if(src==NULL){
	
		return NULL;
	}

	while(	*(dst++)=*(src++)){
		
		i++;	
		printf("i->%d\n",i);
	}

	return tmp;
}


//将第二个字符串的n个字节拷贝出来
char *mystrncpy(char *dst,char *src,int n)
{
	char *tmp=dst;
	int i=0;
	
	if( (src==NULL)||(n<0)){
		
		return NULL;
	}
	
	while( (*(dst++) = *(src++))&&(i<n-1) ){
	
		i++;
		printf("i->%d\n",i);
	}

	return tmp;
}


//字符串连接函数，将第二个字符串连接到第一个上
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

//将字符串前n个字节拷贝到前面
char *mystrncat(char *dst,char *src,int n)
{
	char *tmp=dst;
	int i=0,j=0;
	
	if((src==NULL)||(n<0)){
	
		return NULL;
	}
	
	while(dst[i]!='\0'){
	
		printf("i->%d\n",i);
		i++;
	}

	while( (dst[i]=src[j])&&(j<n-1)){
	
		printf("i->%d,j->%d\n",i,j);
		i++;
		j++;
	}

	return dst;
}



int main(char *argv[],int argc)
{

#if  1	
	char dst[20]="hello";
	char *src="world";
	
	mystrncat(dst,src,3);
	printf("dst->%s\n",dst);


#else
	//strcpy
	char dst[20]="hello";
	char *src="world";

	mystrncpy(dst,src,3);
	printf("dst->%s\n",dst);

	//strcmp
	char *str1="hello world";
    char *str2="hella";
    int ret;
    
    ret=mystrncmp(str1,str2,7);
    printf("ret->%d\n",ret);

	//strlen
    int len1=0,len2=0;
    char *str="hello world";    

    len1=mystrlen(str);
    len2=strlen(str);

    printf("len1->%d,len2->%d\n",len1,len2);
    
#endif    

	return 0;
}
