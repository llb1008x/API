/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com
	> Created Time: 2017年10月15日 星期日 23时17分45秒

功能：
   将输入中包含特定模式或字符串的各行打印出来

输入的字符串
   Ah Love! could you and i with fate conspire
   To grasp this sorry scheme of things entire
   would not we shatter it to bits --and then
   Re-mould it nearer to the heart's desire!
************************************************************************/

#include <stdio.h>

#define MAX_SIZE    1000
/*
char src_string[]=
"  Ah Love! could you and i with fate conspire \
   To grasp this sorry scheme of things entire \
   would not we shatter it to bits --and then  \
   Re-mould it nearer to the heart's desire!"
;
*/
char dest_string[]="ould";

//字符串匹配，t在s中下表的位置
int strindex(char *s,char *t)
{
    int i,j,k;

    for(i=0;s[i] != '\0';i++){
        
        //字符串匹配
        for(j=i,k=0;(s[j]==t[k]) && (t[k]!='\0' );j++,k++)
                    ;
        //匹配到了
        if((t[k]=='\0') && k>0){
            
            return i;
        }
    }
    //没有匹配到
    return -1;
}


//返回字符串t在S中最右边的位置
int strindex_right(char *s,char *t)
{
    int i,j,k,ret;

    for(i=0;s[i] != '\0';i++){
        
        //字符串匹配
        for(j=i,k=0;(s[j]==t[k]) && (t[k]!='\0' );j++,k++)
                    ;
            //匹配到了
            if((t[k]=='\0') && k>0){
                //每匹配到一个值，把最新的位置赋给ret          
                ret=i;
        }
    }
    
    //没有匹配到
    if( (ret==0)&&(k==0)){
        
        ret=-1;
    }
    
    
    return ret;

}



//将行保存到s中，并返回该行的长度
int mygetline(char *s,int lim)
{
    int i=0,c;
    
    while( (--lim>0) && ((c=getchar()) != EOF ) &&(c!='\n') ){

        s[i++]=c;
    }

    if(c=='\n'){

        s[i++]=c;
    }

    s[i]='\0';

    return i;
}



int main()
{
    //printf("src_string->%s\n",src_string);
    int found=0,ret=0;
    char src_string[MAX_SIZE]="";
    
    while(mygetline(src_string,MAX_SIZE) > 0){
        
        if( (ret=strindex_right(src_string,dest_string)) >= 0){

            printf("%s\n",src_string);
            printf("ret->%d\n",ret);
            found++;
        }
    }

    printf("found->%d\n",found);

    return 0;
}
