/*************************************************************************
	> File Name: test2.c
	> Author: 
	> Mail: 
	> Created Time: 2018年01月11日 星期四 20时41分45秒
 ************************************************************************/

#include <stdio.h>
#include <setjmp.h>
#include "apue.h"

static void f1(int i,int j,int k);
static void f2(void);

static jmp_buf jmpbuffer;

int main(int argc,char *argv[])
{
	int count;
	register int val;
	volatile int sum;
	
	count=2;
	val=3;
	sum=4;
	
	if(setjmp(jmpbuffer)!=0){
	
		printf("afrer longjmp:count =%d,val=%d,sum=%d\n",
				count,val,sum);
		exit(0);		
	}
	
	count=97;
	val=98;
	sum=99;
	
	f1(count,val,sum);
	
    exit(0);
}


static void f1(int i,int j,int k)
{

	printf("in f1():count=%d,val=%d,sum=%d\n",i,j,k);
	f2();
}

static void f2(void)
{
	longjmp(jmpbuffer,1);
}
