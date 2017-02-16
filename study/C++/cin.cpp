/*****************************************************************************************
 *compare cin ,cin.getline
 * **********************************************************************/
#include <iostream>
#include <string.h>

using namespace std;

int main()
{
    char str1[100];
    char str2[100];

    memset(str1,100,0);
    memset(str2,100,0);

    cin>>str1;
    cin.getline(str2,100);
    
    cout<<str2<<endl;
    cout<<str1<<endl;
    
    return 0;
}
