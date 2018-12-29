/*************************************************************************
	> File Name: array.c
	> Author: llb
	> Mail: llb1008x@126.com
	> Created Time: 2017年08月27日 星期日 20时25分14秒
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "array.h"

#define BLOCK  5

void array_inflate(Array *a,int index);

Array array_create(int init_size)
{
    Array a;
    a.size=init_size;
    a.array=(int *)malloc(sizeof(int )*a.size);
    return a;
}


void array_free(Array *a)
{
    free(a->array);
    a->array=NULL;
    a->size=0;
}


int array_size(const Array *a)
{
    return a->size;
}


int *array_at(Array *a,int index)
{
    if(index>=a->size){

        array_inflate(a,(index/BLOCK+1)*BLOCK-a->size);
    }
    return &(a->array[index]);
}


int array_get(const Array *a,int index)
{
    
    return a->array[index];
}


void array_set(Array *a,int index,int value)
{

    a->array[index]=value;
}


//可变数组的增长
void array_inflate(Array *a,int more_size)
{
    int *p=(int *)malloc(sizeof(int )*(a->size+more_size));
    int i;

    for(i=0;i<a->size;i++){

        p[i]=a->array[i];
    }

    free(a->array);
    a->array=p;
    a->size += more_size;
    
}

int main()
{
    Array a=array_create(100);
    *(array_at(&a,0))=10;
    printf("%d\n",a.array[0]);
    int number=0,i=0;
    
    while(number!=-1){
        
        scanf("%d",&number);
        *array_at(&a,i)=number;
        printf("a->array[%d]=%d\n",i,array_get(&a,i));
        i++;
    }

    array_free(&a);

    return 0;
}
