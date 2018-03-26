
#include <stdio.h>

void inverse(char *p)                     
{
    if( *p == '\0' )
        return;

    inverse( p+1 );   //切记：对于字符串的指针加1，就是指向最后一个字符

    printf( "%c", *p );

}

 

int main(int argc, char *argv[])
{
    char *str="hello";
    
    inverse(str);
    printf("str->%s\n",str);


    return 0;
}
