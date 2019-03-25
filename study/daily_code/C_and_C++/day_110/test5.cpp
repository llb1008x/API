/*************************************************************************
	> File Name: test5.cpp
	> Author:llb 
	> Mail: llb1008x@126.com
	> Created Time: 2019年01月22日 星期二 23时39分20秒
 ************************************************************************/

#include<iostream>
#include<string>

using namespace std;

class Student{

    public:
        Student()
        {
            cout<<"student"<<endl;
        }
        //拷贝构造函数
        Student(const Student&)
        {
            cout<<"student2"<<endl;
        }
    private:
        string m_strName;
};

int main()
{
    Student stu1;
    Student stu2=stu1;
    Student stu3(stu1);
    return 0;
}
