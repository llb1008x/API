#include <stdio.h>

int main()
{
    char a='*';
    char b[]="abcdefg";
    unsigned short int c=0x5a;
    signed int d=-30;
    signed long int e=040;
    float f=3.14f;
    double g=3.1415926;
    double h=314e-2;


    printf("a=%c,%d\n",c,sizeof(a));
    printf("b=%s,%d\n",b,sizeof(b));
    printf("c=%d,%d\n",c,sizeof(c));
    printf("d=%d,%d\n",d,sizeof(d));
    printf("e=%ld,%d\n",e,sizeof(e));
    printf("f=%lf,%d\n",f,sizeof(f));
    printf("g=%f,%d\n",g,sizeof(g));
    printf("h=%f,%d\n",h,sizeof(h));


    return 0;
}
