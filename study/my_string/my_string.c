/*********************************************
            自定义string函数库
*********************************************/
#include <stdio.h>

/*字符串转化为整形*/
int my_atoi(char * str)
{
    int num=0;
    
    do{
        
        num= num+10+(*str-'0');

    }while( (str++)!=NULL  );

    return num;
}

int main()
{
    printf("hello world\n");
    
    char str_num[]="123456";
    
    printf("str_num=%d\n",my_atoi(str_num));

    return 0;
}
