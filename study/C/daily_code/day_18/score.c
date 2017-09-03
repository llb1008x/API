/*************************************************************************
	> File Name: score.c
	> Author: llb
	> Mail: llb1008x@126.com
	> Created Time: 2017年05月20日 星期六 16时48分31秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>

char *fun(int score)
{
     char *str=(char *)malloc(10);   

    if((score<0)||(score>100)) {

        str="error";
    }else if( (score>=0) && (score<=59) ){

        str="E";
    }else if(score>=60 && score<=69 ){

        str="D";
    }else if((score>=70) && (score<=79) ){

        str="C";
    }else if((score>=80) && (score<=89) ){

        str="B";
    }else if(score>90 ){

        str="A";
    }

    return str;
}
