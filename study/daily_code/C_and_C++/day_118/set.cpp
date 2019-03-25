/*************************************************************************
	> File Name: set.cpp
	> Author:llb 
	> Mail: llb1008x@126.com
	> Created Time: 2019年03月06日 星期三 23时02分52秒
 ************************************************************************/

#include<iostream>
#include<set>

using namespace std;

int main()
{
    set<int>s;
    s.insert(1);
    cout<<*(s.begin())<<endl;
    
    for(int i=0;i<6;i++)
        s.insert(i);

    for(auto it=s.begin();it!=s.end();it++)
        cout<<*it<<" ";

    cout<<endl<<(s.find(2)!=s.end())<<endl;

    cout<<endl<<(s.find(10)!=s.end())<<endl;

    s.erase(1);
    cout<<(s.find(1)!=s.end())<<endl;

    return 0;
}
