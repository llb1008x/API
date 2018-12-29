/*************************************************************************
	> File Name: test2.c
	> Author: 
	> Mail: 
	> Created Time: 2017年11月17日 星期五 19时16分37秒
	
将字符串转化成浮点型
 ************************************************************************/

#include <stdio.h>
#include <ctype.h>
#include <math.h>

double myatof(char s[])
{
	double val,power;
	int i,sign,esign,eindex=0;
	long Enum;
	double ret;
	

	//skip space
	for(i=0;isspace(s[i]);i++)
			;
	//处理符号	
	sign=(s[i]=='-')?-1:1;
	if(s[i]=='+' || s[i]=='-'){
	
		i++;
	}		
	
	//处理整数部分,isdigit检测字符串是否由数字组成
	for(val=0.0;isdigit(s[i]);i++){
	
		val=10.0*val+(s[i]-'0');
	}
	printf("1 val->%f\n",val);
	//小数点
	if(s[i]=='.'){
		
		i++;
	}
	
	//处理小数部分 
	for(power=1.0;isdigit(s[i]);i++){
		
		val=10.0*val+(s[i]-'0');
		power *= 10;
	}
	printf("2 val->%f,power->%f\n",val,power);
	printf("num->%f,i->%d\n",sign*val/power,i);
	
	
	
	//处理是否有E指数
	if(s[i]=='e' || s[i]=='E'){
	
        
        i++;
        
        esign=(s[i]=='-')?-1:1;
        if(esign<0){
       
                i++;
        }
        
        //提取E以后的数
        while(s[i]!='\0'){
            printf("i->%d,eindex->%d\n",i,eindex);
            eindex=eindex*10+(s[i]-'0');
            i++;
        }
        
        printf("eindex->%d\n",eindex);
        
        Enum=pow(10,eindex);
        printf("Enum->%ld\n",Enum);
	}
	
	
	if(esign > 0){
	    printf("aaa\n");
	    ret=sign*val/power*Enum;
	}else{
	    printf("bbb\n");
	    ret=sign*val/power/Enum;
	}
	
	return ret;
}

int main()
{
 	char str[]="123.45E-3";
 	double num;
	
	num=myatof(str);
	printf("str->%s,num->%f\n",str,num);

    return 0;
}
