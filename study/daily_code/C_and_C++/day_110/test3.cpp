#include <iostream>
#include <string>

using namespace std;

int main()
{
    string name;
    
    cout <<"please input your name:";
    
    getline(cin,name);
    
    cout<<"hello "+ name<<endl;

    cout<<"your first name letter is :"<<name[0]<<endl;

    if(name.empty()){
        cout<<"input is null..."<<endl;
        return -1;
    }

    if(name == "imooc")
        cout<<"you are an administrator"<<endl;

    
    return 0;
}