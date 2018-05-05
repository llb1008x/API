//这个程序可以用于大端小端的判断
#include <stdio.h>

int main()
{
    int a = 0x1234,b=0x5678;
    char *p1,*p2;
    
    printf("1-%0x %0x \n",a,b);
    
    p1=(char *)&a;
    p2=(char *)&b;
    printf("2-%0x %0x\n",*p1,*p2);
    
    p1++;
    p2++;
    printf("3-%0x %0x\n",*p1,*p2);

    return 0;
}
