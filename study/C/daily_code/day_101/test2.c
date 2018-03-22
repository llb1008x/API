/*************************************************************************
	> File Name: test2.c
	> Author: 
	> Mail: 
	> Created Time: 2018年03月07日 星期三 14时28分19秒
 ************************************************************************/

#include <stdio.h>


int add_n(int n)
{

    static int i = 100;

    i += n;

    return i;

}

int main()
{
	printf("add_n->%d\n",add_n(add_n(2)));
	//printf("add_n->%d\n",add_n(3));
	
#if 0
	unsigned short A = 10;

	printf("~A = %u\n", ~A);

	 

	char c=128;

	printf("c=%d\n",c);
#endif 

    return 0;
}
