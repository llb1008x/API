/*

利用异或运算对输入的字符串进行加密
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEY 0x86

int main()
{
    char p_data[20]={"hello world"};
    char encrpy[20]={0},decode[20]={0};
    int i;
    
    for(i=0;i<strlen(p_data);i++)
            encrpy[i]=p_data[i]^KEY;
    for(i=0;i<strlen(encrpy);i++)
            decode[i]=encrpy[i]^KEY;
            
    printf("inital->%s\n",p_data);        
    printf("encrpy->%s\n",encrpy);
    printf("decode->%s\n",decode);

    return 0;
}
