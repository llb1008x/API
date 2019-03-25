/*************************************************************************
	> File Name: pat13.cpp
	> Author:llb 
	> Mail: llb1008x@126.com
	> Created Time: 2019年02月25日 星期一 23时31分21秒
6-13 折半查找 （15 分）
给一个严格递增数列，函数int Search_Bin(SSTable T, KeyType k)用来二分地查找k在数列中的位置。
 ************************************************************************/

#include<iostream>
using namespace std;

#define MAXSIZE 50

typedef int KeyType;

typedef struct{
    KeyType key;
}ElemType;

typedef struct{
    ElemType *R;
    int length;
}SSTable;

void Create(SSTable &T)
{
    int i;
    
    T.R=new ElemType[MAXSIZE+1];
    cin>>T.length;
    for(i=1;i<=T.length;i++){
        cin>>T.R[i].key;
    }
}

int Search_Bin(SSTable T,KeyType k);

int main()
{
    SSTable T;
    KeyType k;
    
    Create(T);
    cin>>k;
    int pos=Search_Bin(T,k);
    if(pos==0)
        cout<<"NOT FOUND"<<endl;
    else
        cout<<pos<<endl;
    
    return 0;
}


int Search_Bin(SSTable T,KeyType k)
{
    int low=0,high=T.length-1,mid;
    
    while(low<=high){
        mid=(low+high)/2;
        
        if(T.R[mid].key==k)
            return mid;
            
        if(T.R[mid].key<k)
            low=mid+1;
        
        if(T.R[mid].key>k)
            high=mid-1;
                   
    }
    
    if(low>high)
        return 0;
     else
        return mid;   
}
