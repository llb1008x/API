/*************************************************************************
	> File Name: array.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年07月04日 星期二 08时42分32秒
 ************************************************************************/

#include "array.h"
#include <stdlib.h>

/*
 *关于这个create还有很多要思考的内容
 * */
Array array_create(int init_size)
{
	Array a;
	
	a.size=init_size;
	a.array=(int *)malloc(sizeof(int)*a.size);

	return a;
}

/*
 *	主要是清空*array这个指针
 * */
void array_free(Array *a)
{
	free(a->array);
	a->array=NULL;
	a->size=0;
}


int array_size(Array *a)
{

	return a->size;
}


/*
 *
 * */
int *array_at(Array *a,int index)
{
	return &(a->array[index]);
}


void array_inflate(Array *a,int more_size)
{

}
