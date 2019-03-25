/*************************************************************************
	> File Name: bitset.cpp
	> Author:llb 
	> Mail: llb1008x@126.com
	> Created Time: 2019年03月06日 星期三 23时24分26秒
 ************************************************************************/

#include<iostream>
#include<bitset>


using namespace std;

int main()
{
    bitset<5>b("11");

    for(int i=0;i<5;i++)
        cout<<b[i];

    cout<<endl<<b.any();
    cout<<endl<<b.none();
    cout<<endl<<b.count();
    cout<<endl<<b.size();
    cout<<endl<<b.test(2);

    b.set(4);
    b.reset();
    b.reset(3);
    b.flip();

    unsigned long a=b.to_ulong();

    return 0;
}

