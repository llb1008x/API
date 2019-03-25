/*
C++ 类的封装
*/ 
#include <iostream>
#include <string>
using namespace std;

class Student{

public:
    void setname(string _name)
    {
        m_strName = _name;
    }
    string getname()
    {
        return m_strName;
    }

    void setgender(string _gender)
    {
        m_strGender=_gender;
    }
    string getgender()
    {
        return m_strGender;
    }

    int getscore()
    {
        return m_iScore;
    }
    
    void study(int _score)
    {
        m_iScore += _score;
    }

    int init(int _score)
    {
        m_iScore=_score;
    }


private: 
    string m_strName;
    string m_strGender;
    int m_iScore;

};


int main()
{
    Student stu;
    stu.init(90);
    stu.setname("baoge");
    stu.setgender("man");
    
    cout<<stu.getname()<<" "<<stu.getgender()<<" "<<endl;
    cout<<stu.getscore()<<endl;

    return 0;
}
