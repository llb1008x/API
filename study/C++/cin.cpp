#include <iostream>
#include <string.h>

using namespace std;

int main()
{
    char str1[10],str2[10];

    memset(str1,10,0);
    memset(str2,10,0);

    cin.getline(str1,10);
    cin.getline(str2,10);
   
    cout<<"str1"<<str1<<endl;
    cout<<"str2"<<str2<<endl;


    return 0;
}
