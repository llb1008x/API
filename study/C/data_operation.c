#include <stdio.h>

int main()
{
 
#if  1

    
#else
    int i=6;
    float j=3.6f;

    i=j;
    i=(int )j;

    printf("%d,%lf\n",i,j);
#endif

    return 0;
}
