/*************************************************************************
> File Name: test2.cpp
> Author:llb
> Mail: llb1008x@126.com
> Created Time: 2019年01月22日 星期二 17时03分59秒
************************************************************************/

#include<iostream>
#include<stdlib.h>
#include<string>

using namespace std;


class Coordinate{

public:
	int x;
    int y;

	void printX()
        {
		cout<<x<<endl;
	}
	void printY()
	{
		cout<<y<<endl;
	}
};

int main() 
{
	cout << "hello world" << endl;

	Coordinate *p =new Coordinate();
	p->x=100;
	p->y=200;

	p->printX();
	p->printY();
	//system("pause");	

	delete p;

	return 0;
}