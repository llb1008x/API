/*************************************************************************
	> File Name: test.c
	> Author: llb
	> Mail: llb1008x@126.com
	> Created Time: 2017年06月25日 星期日 09时05分32秒
    指针的应用
    1.指针变量在没有指向任何变量之前，不要做赋值操作
    
    2.当要传递的参数的类型比地址要大的时候，这时常用的手段：
    既能用比较少的字节数传递值给参数，又能避免函数对外面的变量的修改
    
    3.把非const转换位const
    void f(const int *x);
    int a=15;
    f(&a)
    
    4.判断哪个被const了的标志是const在*前面还是后面，
    指针也是一样，*修饰的是谁
    int i;
    const int *p1=&i;
    int const *p2=&i;
    int *const *p3=&i;

    5.传入函数的数组参数实际是指针，所以sizeof(a),得到的是地址
    f(int a[])  =  f(int *a)
    数组是特殊的指针，const指针

    6.数组全部赋值必须在初始化的时候，之后只能单个赋值
    const数组   const int a[]={1,2,5,7,9};
    数组变量已经是const的指针了，这里的const表明数组的每个单元都是const int

 ************************************************************************/

#include <stdio.h>

/*
*指针应用一：
*函数要返回多个值
* */
void swap(int *a,int *b)
{
    int t=*a;
    *a=*b;
    *b=t;
}

void minmax(int a[],int len,int *min,int *max)
{
    int i;
    *min=*max=a[0];

    for(i=1;i<len;i++){

        if(a[i]<*min){
            *min=a[i];
        }

        if(a[i]>*max){
            *max=a[i];
        }
    }
}


/*
*指针应用二：
*函数返回值用于判断状态，结果通过指针返回
* */
int divide(int a,int b,int *result)
{
    int ret=1;

    if(b==0){

        ret=0;
    }else{

        *result=a/b;
    }
    
    return ret;
}


int main()
{
/*    
    int a=1,b=4;
    swap(&a,&b);
    printf("a->%d,b->%d\n",a,b);


    int a[]={1,2,3,4,5,6,7,8,9,1,0,5,44,26,236};
    int min,max;
    int len=sizeof(a)/sizeof(a[0]);

    minmax(a,len,&min,&max);
    printf("min->%d,max->%d\n",min,max);
*/

    int a=5,b=2;
    int c;

    if(divide(a,b,&c)){
        
        printf("a->%d,b->%d,c=a/b=%d\n",a,b,c);
    }

    return 0;
}
