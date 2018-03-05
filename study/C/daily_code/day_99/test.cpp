/*************************************************************************
	> File Name: test.cpp
	> Author: 
	> Mail: 
	> Created Time: 2018年03月05日 星期一 14时50分59秒
 ************************************************************************/

#include <iostream>
#include <string.h>

using namespace std;

typedef struct mystruct{
    
    double dda1;
    char dda;
    int type;
}MYSTRUCT;

void test(int ptr[],int *e)
{
    int a =  sizeof(ptr);
    char p[]="hello";
    int b=sizeof(p);
    int c=strlen(p);

    cout<<a<<" "<<b<<" "<<c<<" "<<sizeof(e)<<endl;
}

int main()
{
    int ptr[8]={0};
    int a;
    test(ptr,&a);

    cout<<sizeof(MYSTRUCT)<<endl;
    cout<<sizeof(float)<<endl;

    return 0;
}
