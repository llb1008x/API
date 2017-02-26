/*********************************************************
		cin ,getline的区别
cin是以空格，tab，换行为结束的
getline是以换行为结束
*********************************************************/
#include <iostream>

using namespace std;


int main()
{

	char name1[20]="",name2[20]="";

#if    0
	cin>>name1>>name2;

#else
	cin.getline(name1,20);
	cin.getline(name2,20);
		

#endif
	cout<<"name1:"<<name1<<"name2:"<<name2<<endl;


	return 0;
}
