/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年08月07日 星期一 20时52分10秒
 ************************************************************************/
#include <stdio.h>
#include <string.h>



//计算字符串的长度
int my_strlen(char *str)
{
    int i=0;

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
int my_strcmp(char *str1,char *str2)
{
    int ret=0,i=0;
    
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


int main()
{

#if  1
    char *str1="hella";
    char *str2="hella";
    int ret;
    
    ret=my_strcmp(str1,str2);
    printf("ret->%d\n",ret);


#else
    int len1=0,len2=0;
    char *str="hello world";    

    len1=my_strlen(str);
    len2=strlen(str);

    printf("len1->%d,len2->%d\n",len1,len2);
    
#endif    

	return 0;
}
