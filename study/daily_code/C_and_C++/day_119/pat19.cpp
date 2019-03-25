/*

1004 成绩排名 （20 分）
读入 n（>0）名学生的姓名、学号、成绩，分别输出成绩最高和成绩最低学生的姓名和学号。

输入格式：
每个测试输入包含 1 个测试用例，格式为

第 1 行：正整数 n
第 2 行：第 1 个学生的姓名 学号 成绩
第 3 行：第 2 个学生的姓名 学号 成绩
  ... ... ...
第 n+1 行：第 n 个学生的姓名 学号 成绩
其中姓名和学号均为不超过 10 个字符的字符串，成绩为 0 到 100 之间的一个整数，这里保证在一组测试用例中没有两个学生的成绩是相同的。

输出格式：
对每个测试用例输出 2 行，第 1 行是成绩最高学生的姓名和学号，第 2 行是成绩最低学生的姓名和学号，字符串间有 1 空格。

输入样例：
3
Joe Math990112 89
Mike CS991301 100
Mary EE990830 95
输出样例：
Mike CS991301
Joe Math990112

*/

#include <iostream>
#include <string>
#include <vector>

using namespace std;

//学生对象,姓名，学号，成绩
struct student{
    char Name[10];
    char Cnum[10];
    int score;
};

int main()
{
      
    //要输入多少个学生
    int N,max=0,min=0,k,p;
    cin>>N;
    vector<student>stu(N);
    
    //输入学生信息
    //scanf("%s%s%d",stu[0].Name,stu[0].Cnum,stu[0].score);
    for(int i=0;i<N;i++){
       cin>>stu[i].Name>>stu[i].Cnum>>stu[i].score;
       
       //判断最高成绩，最低成绩
       if(max==0 && min ==0){
            max=stu[i].score;
            min=stu[i].score;
       }
       
       
       if(stu[i].score>max){
                max=stu[i].score;
                k=i;
       }         
       if(stu[i].score<min){
                min=stu[i].score;          
                p=i;
       }         
    }
    
    
    
    //输出
    cout<<stu[k].Name<<" "<<stu[k].Cnum<<endl;
    cout<<stu[p].Name<<" "<<stu[p].Cnum<<endl;

    return 0;
}
