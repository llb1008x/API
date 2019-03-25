/*
3.某大桥按不同型号征收车辆过桥费:自行车免费,摩托车2元,小汽车5元,大客车
与货车8元,货柜车12元。编写一个程序,按车辆的不同型号计算通过该大桥应征的过桥费。
(提示:可以用整数对不同型号的车辆进行编码) 
*/


#include <iostream>

using namespace std;

int main()
{
    int type,money;
    cin>>type;
    
    switch(type){
        case 1:
            money=0;
            break;
        case 2:
            money=2;
            break;
        case 3:
            money=5;
            break;
        case 4:
            money=8;
            break;
        case 5:
            money=12;
            break;
        default:
            cout<<"error"; 
            return -1;                   
    }

    cout<<money<<endl;
    
    return 0;
}

