/**********************************************
		�ݹ���ô��ڵ�����
**********************************************/

#include <stdio.h>
#include <string.h>


#if    0
void PrintFun(int N)
{
    int i;

    for(i=1;i<N;i++){
    
	printf("%d\n",i);
    }
    
}

#else 

void PrintFun(int N)
{
   if(N){
   
	PrintFun(N-1);
	printf("%d\n",N);
   } 
}

#endif 

int main(int argc,char *argv[])
{
    PrintFun(atoi(argv[1]));

    return 0;
}
