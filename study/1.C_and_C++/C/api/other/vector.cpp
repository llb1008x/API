/*************************************************************************
	> File Name: vector.cpp
	> Author:llb 
	> Mail: llb1008x@126.com
	> Created Time: 2019年03月06日 星期三 22时50分36秒
 ************************************************************************/

#include<iostream>
#include<vector>

using namespace std;

int main()
{
    vector<int>a;
    cout<<a.size()<<endl;

    for(int i=0;i<10;i++)
        a.push_back(i);

    cout<<a.size()<<endl;

    vector<int>b(15);
    cout<<b.size()<<endl;
    for(int i=0;i<b.size();i++){

        b[i]=15;
    }

    vector<int>c(20,2);
    for(int i=0;i<c.size();i++)
        cout<<c[i]<<" ";

    cout<<endl;

    for(auto it=c.begin();it!=c.end();it++)
        cout<< *it<<" ";

    return 0;
}
