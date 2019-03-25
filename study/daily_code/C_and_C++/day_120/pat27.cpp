/*
  对于N个帽子，求第K便宜的帽子价格，如果不存在则输出-1
*/

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
    int n,k;
    double price;
    //要输入N个数据
    cin>>n;
    
    //输入n个帽子的价格    
    vector<double>V;
    for(int i=0;i<n;i++){
        cin>>price;
        V.push_back(price);
    }    
    
    
    //对vector容器进行排序
    //sort(V.begin(),V.end());   
    //查找第k便宜的帽子
    cin>>k;
    nth_element(V.begin(),V.begin()+k,V.end());
    
    
    for(auto it=V.begin();it!=V.end();it++)
        cout<<*it<<"  ";
        
        
    cout<<endl;        

    if(k<n)
        cout<<V[0]<<endl;
    else
        return -1;    
    //输出第k便宜的帽子
    /*
    double ret;
    ret=find(V.begin(),V.end(),k);
    if(ret==V.end())
        cout<<V[k];
        //cout<<"111"<<endl;
    else
        return -1;    
    */
    return 0;
}
