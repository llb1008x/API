/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年07月04日 星期二 08时31分00秒
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>

#include "array.h"

int main()
{
	Array a=array_create(100);
	printf("size->%d\n",array_size(&a));

	*array_at(&a,0)=10;

	array_free(&a);

	return 0;
}
