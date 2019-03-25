/*
将输入的整数逆序输出
*/

#include <iostream>

using namespace std;

#define MAXSIZE 10   
int stack[MAXSIZE];
int top=-1;
//将整数逆序输出
int rev(int N)
{
    int m=0;
    int ret=0;
    
    while(N!=0){
        m=N%10;
        N=N/10;
        //cout<<"111"<<endl;
        stack[++top]=m;
    }
    
    //打印输出
    for(int i=0;i<=top;i++){
    
        if((stack[i]==0) && (ret==0)){
                //cout<<"222"<<endl;
                continue;
        }        
        if(stack[i]!=0){
            ret=1;        
            //cout<<"111"<<endl;
        }
        cout<<stack[i];
        //cout<<"222"<<endl;
    }
    cout<<endl;
    
    return 1;
}

int main()
{

    //输入整数
    int N;
    cin>>N;
    
    //逆序输出
    rev(N);
    
    return 0;
}
