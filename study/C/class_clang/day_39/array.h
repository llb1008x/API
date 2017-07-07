/*************************************************************************
	> File Name: array.h
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年07月04日 星期二 08时37分22秒
 ************************************************************************/

#ifndef __ARRAY_H__
#define __ARRAY_H__

typedef struct {

	int *array;
	int size;
}Array;

Array array_create(int init_size);

void array_free(Array *a);

int array_size(Array *a);

int *array_at(Array *a,int index);

void array_inflate(Array *a,int more_size);

#endif 

