/*************************************************************************
	> File Name: array.h
	> Author: llb
	> Mail: llb1008x@126.com
	> Created Time: 2017年08月27日 星期日 20时26分20秒
 ************************************************************************/

#ifndef _ARRAY_H
#define _ARRAY_H

typedef struct {

    int *array;
    int size;
}Array;


Array array_create(int init_size);

void array_free(Array *a);

int array_size(const Array *a);

int *array_at(Array *a,int index);

void array_inflate(Array *a,int more_size);

#endif
