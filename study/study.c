#include <stdio.h>

int main()
{
    printf("hello world\n");

#if 1   
    int c=0;
    int num=0;
    while((c=getchar())!='0'){

        if(c=='\n')
            num++;
    }
    printf("num-->%d\n",num);

#else    
    //getchar()
    int c=getchar();

    while(c!='\n'){

        printf("%c",c);
        c=getchar();
    }

    //getchar()!=EOF 1?0
    if(getchar()!=EOF){

        printf("1\n");
    }else{

        printf("0\n");
    }
    
    //input and output
    int c;
    while( (c=getchar())!='\n'){

        putchar(c);
    }

#endif    
        
    return 0;
}
