/*************************************************************************
	> File Name: hello.c
	> Author:llb 
	> Mail: llb1008x@126.com
	> Created Time: 2019年01月21日 星期一 21时03分23秒
 ************************************************************************/

#include<stdio.h>

typedef struct weapon{
    char name[20];
    int size;
    int price;
}WEAPON;

int main()
{
    WEAPON ak47={"baoge",1,100};
    //ak47.name = "baoge";
    //ak47.size=1;
    //ak47.price=100;
    
    printf("ak47orice-%s\n",ak47.name);
    
    return 0;
}
