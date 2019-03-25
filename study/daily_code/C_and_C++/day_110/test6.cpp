/*************************************************************************
	> File Name: test6.cpp
	> Author:llb 
	> Mail: llb1008x@126.com
	> Created Time: 2019年01月23日 星期三 00时00分14秒
 ************************************************************************/

#include<iostream>
#include<string>

using namespace std;

class Student{

    public:
        Student()
        {
            m_pName = new char[20];
        }

        ~Student()
        {
            delete []m_pName;
        }
    private:
        char *m_pName;
};

int main()
{
    return 0;
}
