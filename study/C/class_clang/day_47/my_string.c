/*************************************************************************
	> File Name: string.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年04月25日 星期二 23时24分04秒


自定义实现字符串常用的函数,最初步的想法，虽然很low
version 1.0  2017.8.7  实现最基本的功能 

	strlen
	strcmp/strncmp
	strcpy/strncpy
	strcat/strncat
	strchr 字符匹配
	strstr 字符串匹配
	memset 空间设定
	atoi/atol/atof 字符串转换 
	strtok字符串切割  比较有困难的应该是这个
	
char *strtok(char s[], const char *delim);
功能:strtok()用来将字符串分割成一个个片段。
	参数1:s指向欲分割的字符串
	参数2:delim则为分割字符串中包含的所有字符。
	当strtok()在参数s的字符串中发现参数delim中包含
	的分割字符时,则会将该字符改为\0 字符,当连续出现多个
	时只替换第一个为\0。
	在第一次调用时:strtok()必需给予参数s字符串
	往后的调用则将参数s设置成NULL,每次调用成功则返
	回指向被分割出片段的指针

	
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include "my_string.h"

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


//字符匹配
char *mystrchr(char *src,char ch)
{
    while( *src!='\0'){
    
        if(*(src++)==ch){
        
            return src;
        }
    }
        
    if(src==NULL)
         return NULL;    
   
}


//字符串匹配
#if     1
char *mystrstr(char *str1,char *str2)
{
    //char *tmp=str2;
    int i=0;

    if(str1==NULL){
    
        return NULL;
    }
    
    printf("11111111\n");
    while(str1!=NULL){
        
        printf("222222222\n");
        while(str2[i]!='\0'){
            
            printf("i->%d\n",i);
            printf("33333333\n");
            if(str1[i]!=str2[i]){
            
                printf("44444444\n");
                printf("incompartable\n");
                break;
            }
            i++;
        }
        
        if(str2[i]=='\0'){
            
            printf("555555555\n");
            printf("compartable\n");
            return str1;
        }else{
        
            printf("66666666\n");
            str1++;
            i=0;
        } 
    }
      
    if(str1==NULL){
    
        printf("7777777777\n");
        return NULL;
    }    

}

#else 

char *mystrstr(char *str1,char *str2,int len1,int len2)
{
    int i,j;
    
    
    for(i=0;i<len1;i++){
    
        printf("111111111\n");
        for(j=0;j<len2;j++){
            
            if(str1[i+j]!=str2[j]){
                
                printf("222222222\n");
                break;
            }
            
            if(j==len2-1){
            
                printf("匹配成功\n");
                return str1+i;
            }
            
            printf("333333333\n");
        }
        printf("4444444444\n");
    }
    
    return NULL;
}
#endif 



//字符串转
int myatoi(char *str)
{
    int num=0,i=0;
    
    while( str[i]!='\0'){
    
        if( (str[i]>='0')&&(str[i]<='9'))
        num = num*10+str[i]-'0';
        i++;     
    }
   
    return num;
}



