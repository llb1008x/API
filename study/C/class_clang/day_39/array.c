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
