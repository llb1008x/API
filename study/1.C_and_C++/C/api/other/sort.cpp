/*************************************************************************
	> File Name: sort.cpp
	> Author:llb 
	> Mail: llb1008x@126.com
	> Created Time: 2019年03月06日 星期三 23时29分22秒
 ************************************************************************/

#include<iostream>
#include<algorithm>
#include<vector>

using namespace std;

bool cmp(int a,int b)
{

    return a>b;  //从大到小排列
}

int main()
{
    vector<int>v(10);
    for(int i=0;i<10;i++){

        cin>>v[i];
    }

    sort(v.begin(),v.end());

    int arr[10];
    for(int i=0;i<10;i++)
        cin>>arr[i];

    sort(arr,arr+10,cmp);

    return 0;
}
