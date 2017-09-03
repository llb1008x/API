/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年08月25日 星期五 19时50分38秒


1.可变数组
	
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//数据结构
typedef struct{
	
	int *array;
	int size;
}Array;


//创建可变数组
Array array_create(int init_size)
{
	Array a;
	
	a.array=(int *)malloc(sizeof(int )*init_size);
	a.size=init_size;
	return a;

}

//释放申请的空间
void array_free(Array *a)
{
	free(a->array);
	a->array=NULL;
	a->size=0;
}


//获取空间的大小
int array_size(const Array *a)
{
	return a->size;
}


//动态增长
#if 	1
void array_inflate(Array *a,int more_size) 
{
	int *P=(int *)malloc(sizeof(int )*(a->size+more_size));
	int i=0;

	for(i=0;i< a->size;i++)	{
		
		P[i]=a->array[i];
	}

	//先释放之前的空间，然后重新申请一整块空间
	free(a->array);

	a->array=P;
	a->size=a->size+more_size;	
}

#else

void array_inflate(Array *a,int more_size)
{
	int *p=(int *)malloc(sizeof(int )*(a->size+more_size));
	
	memcpy((void *)p,(void *)a->array,a->szie*sizeof(int ));
	
	free(a->array);
	
	a->array=p;
	a->size=a->size+more_szie;

}



#endif 


//获取数组index为的地址
int *array_at(Array *a,int index)
{
	//如果数组大小不够就在申请
	if(index >= a->size){
	
		array_inflate(a,index - a->size);
	}	
	
	return &(a->array[index]);
}





int main()
{
	return 0;
}

