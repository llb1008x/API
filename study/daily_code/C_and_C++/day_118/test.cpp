/*************************************************************************
	> File Name: test.cpp
	> Author:llb 
	> Mail: llb1008x@126.com
	> Created Time: 2019年03月06日 星期三 22时12分19秒
 ************************************************************************/

#include<iostream>
#include<string>
#include<vector>

using namespace std;

void fun(int &n)
{

    n=99;
}


int main()
{
    cout<<"hello world\n";

    vector<int> v1(10,1);

    for(auto i=v1.begin();i!=v1.end();i++)
        cout<<*i<<" ";

#if 0 
    int n=0;
    fun(n);
    cout<<n<<endl;

    vector<int>v1(3,1);
    cout<<v1.size()<<v1[0]<<endl;
    
    string str;
    
    //cin>>str;
    //cout<<str;
      
    getline(cin,str);
    cout<<str.length()<<endl;

    string s2,s3;

    s2=str.substr(5);
    s3=str.substr(4,3);

    cout<<s2<<" "<<s3<<endl;
#endif

    return 0;
}
