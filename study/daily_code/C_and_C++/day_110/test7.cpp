#include <iostream>
#include <string>
using namespace std;
/**
 * 定义类：Student
 * 数据成员：m_strName
 * 无参构造函数：Student()
 * 有参构造函数：Student(string _name)
 * 拷贝构造函数：Student(const Student& stu)
 * 析构函数：~Student()
 * 数据成员函数：setName(string _name)、getName()
 */
 
 class Student{
   
   public:
        Student();
        Student(string _name);
        Student(const Student &stu);
        ~Student();
        void setName(string _name);
        string getName();
        
   private:
        string m_strName;     
 };
 
 
 /*构造函数*/
 Student::Student()
 {
     m_strName = "baoge";
 }

 Student::Student(string _name)
 {
     m_strName = _name;
 }
 
 Student::Student(const Student &stu)
 {
    m_strName = "baoge1";
 }
 
 /*析构函数*/
 Student::~Student()
 {
     cout<<"~Student"<<endl;
 }
 
 /*属性封装*/
 void Student::setName(string _name)
 {
     m_strName=_name;
 }
 
 string Student::getName()
 {
     return m_strName;
 }


int main(void)
{
    // 通过new方式实例化对象*stu
    Student *stu = new Student();
    // 更改对象的数据成员为“慕课网”
	stu->setName("慕课网");
    // 打印对象的数据成员
	cout<<stu->getName()<<endl;
	return 0;
}
