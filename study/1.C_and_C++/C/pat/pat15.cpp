/*
1002 写出这个数 （20 分）
读入一个正整数 n，计算其各位数字之和，用汉语拼音写出和的每一位数字。

输入格式：
每个测试输入包含 1 个测试用例，即给出自然数 n 的值。这里保证 n 小于 10
​100

输出格式：
在一行内输出 n 的各位数字之和的每一位，拼音数字间有 1 空格，但一行中最后一个拼音数字后没有空格。

输入样例：
1234567890987654321123456789
输出样例：
yi san wu




目前显示答案错误.....
*/

#include <iostream>
#include <string.h>

using namespace std;


#define MAXNUM 100

char *Num[]={
    "ling ",
    "yi ",
    "er ",
    "san ",
    "si ",
    "wu ",
    "liu ",
    "qi ",
    "ba ",
    "jiu "
};


//存储转换的数字
int stack[100]={0};
int top=-1;

int main()
{
    //输入字符串
    char N[MAXNUM];
    //输出字符串 组包
    char str[MAXNUM];
    memset(str,0,MAXNUM);
    //字符串数字转换
    int count=0,i=0;
    //字符串长度
    int len=0;
    
    scanf("%s",N);
    printf("a->%s,%d\n",N,atoi(N));
    
    if(atoi(N)==0)
        printf("lin\n");
         
   
    
    while(N[i]!='\0'){
    
        if( (N[i]>='0') && (N[i]<='9'))
            count = count +N[i]-'0';
        i++;
    }
    
    printf("count->%d\n",count);
    
    //转换
    while(count!=0){
        //printf("%s\n",Num[count%10]);
        //printf("%d ",Num[count%10]);
        stack[++top]=count%10;
        count /= 10;  
    }
    
    
    //输出转换的数字
    while(top!= -1){
        strcat(str,Num[stack[top--]]);
        //printf("%s ",Num[stack[top--]]);
    }
    len=strlen(str);
    str[len-1]='\0';
    printf("%s",str);
    
    return 0;
}



//网上的代码
#if 0

#include<iostream>
#include<cstring>
using namespace std;
#define N 1000
 
void show(int sum);
 
int main(){
	int i;
	int sum=0,len;
	int num[N];
	string str;
	cin>>str;
	len=str.length();
	
	for(i=0;i<len;i++){
		num[i]=str[i]-'0';
		sum+=num[i];
	}
	show(sum);
	return 0;
}
void show(int sum){
	int i,j=0,m,n;
	int len;   //字符串的长度 
	int a[500];//来存储和的每一位，逆序存储 
	char b[500];
	
	m=sum; 
	while(m!=0){
		n=m%10;
		a[j++]=n;
		m/=10;
	}
 
	for(i=j-1;i>=0;i--){
		if(a[i]==0){
			strcat(b,"ling ");
		}
		else if(a[i]==1){
			strcat(b,"yi ");
		}
		else if(a[i]==2){
			strcat(b,"er ");			
		}
		else if(a[i]==3){
			strcat(b,"san ");
		}
		else if(a[i]==4){
			strcat(b,"si ");			
		}
		else if(a[i]==5){
			strcat(b,"wu ");
		}
		else if(a[i]==6){
			strcat(b,"liu ");
		} 
		else if(a[i]==7){
			strcat(b,"qi ");
		}
		else if(a[i]==8){
			strcat(b,"ba ");
		}
		else strcat(b,"jiu ");
	}	
	len=strlen(b);
	b[len-1]='\0';
	printf("%s",b);
		
}

#endif

