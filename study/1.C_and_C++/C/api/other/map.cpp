/*************************************************************************
	> File Name: map.cpp
	> Author:llb 
	> Mail: llb1008x@126.com
	> Created Time: 2019年03月06日 星期三 23时11分42秒
 ************************************************************************/

#include<iostream>
#include<map>
#include<string>


using namespace std;
int main()
{
    map<string,int>m;
    
    m["hello"]=2;
    cout<<m["hello"]<<endl;
    cout<<m["world"]<<endl;

    m["world"]=3;
    m[","]=1;


    for(auto it=m.begin();it!=m.end();it++){
        cout<<it->first<<" "<<it->second<<endl;
    }

    cout<<m.size()<<endl;

    return 0;
}
