/*C++ 类的封装*/

#include <iostream>
#include <string>

using namespace std;

/*tacher类，
数据成员，姓名，性别，年龄*/
class Teacher{

public:
    void setName(string _name);
    string getName();
    void setGender(string _gender);
    string getGender();
    void setAge(int _age);
    int getAge();
    void teach();

private:
    string m_strName;
    string m_strGender;
    int m_iAge;
};

void Teacher::setName(string _name)
{
    m_strName = _name;
}

string Teacher::getName()
{
    return m_strName;
}

void Teacher::setGender(string _gender)
{
    m_strGender = _gender;
}

string Teacher::getGender()
{
    return m_strGender;
}

void Teacher::setAge(int _age)
{
    m_iAge = _age;
}

int Teacher::getAge()
{
    return m_iAge;
}

void Teacher::teach()
{
    cout<<"开始上课"<<endl;
}

int main()
{
    Teacher t;
    t.setName("孔子");
    t.setGender("男");
    t.setAge(30);

    cout<<t.getName()<<" "<<t.getAge()<<" "<<t.getGender()<<endl;
    t.teach();
    
    return 0;
}
