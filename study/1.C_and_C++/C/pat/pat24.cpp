/*
2.编写一个程序,用户输入年份及月份两个数据,程序输出该月份的天数。
(提示:对2月要考虑是否闰年,闰年年份要么能被4整除且不能被100整除,
要么能被400整除,除次之外都不是闰年)。
*/

#include <iostream>

using namespace std;

//判断闰年
//#define ISHEAP(x) 
bool is_heap(int year)
{
    if(year%400==0 || ((year%4==0)&&(year%100!=0) ))
            return true;
    else
            return false;        
}

int month_day[]={
    31,
    28,
    31,
    30,
    31,
    30,
    31,
    31,
    30,
    31,
    30,
    31
    
};

int main()
{
    int year,month,day=0;
    //输入年月
    cin>>year>>month;
    if(month<1 || month>12){
        cout<<"error"<<endl;
        return -1;
    }
       
    if(is_heap(year))
        month_day[1]=29;   
    //cout<<year<<" "<<month<<endl;
    //cout<<is_heap(year)<<endl;

    //计算该月份有多少天
    for(int i=1;i<=month;i++){
        day += month_day[i-1];
    }
    
    cout<<day<<endl;
    

    return 0;
}

