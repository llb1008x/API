/*

1.利用异或运算对输入的文本进行加密解密输出,用户输入一个文本(字符串,设不超过

20个字符),然后输入作为密钥的字符,程序输入加密及解密的字符串。
*/

#include <iostream>
#include <string>
#include <vector>
#include <cstdio>

using namespace std;

int main()
{
    //a是字符串，b是加密的字符
    //string a,b,key;
    char a[21],b,key;
    
    cout<<"请输入人字符串和秘钥: ";
    cin>>a;
    cin>>b;
    
    for(int i=0;a[i]!=0;i++)
        a[i]=a[i]^b;
    
    cout<<"加密后：";
    for(int i=0;a[i]!=0;i++)
        cout<<a[i];
        
    cout<<endl<<"请输入密码:";
    cin>>key;
    
    for(int i=0;a[i]!=0;i++)
        a[i]=a[i]^key;    
    for(int i=0;a[i]!=0;i++)
        cout<<a[i];
        
    cout<<endl;            

    return 0;
}


