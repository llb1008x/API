/*************************************************************************
	> File Name: test3.cpp
	> Author: 
	> Mail: 
	> Created Time: 2018年03月05日 星期一 17时16分25秒
 ************************************************************************/

#include <iostream>
using namespace std;

void record(int* pData, unsigned int length,bool (*func)(int));  
bool isEven(int n);  
  
void record(int* pData, unsigned int length,bool (*func)(int))  
{  
  if (pData == NULL || length == 0)  
      return;  

  int *pBegin = pData;  
  int *pEnd = pData+length-1;  

  while(pBegin < pEnd)  
  {  
          while (!func(*pBegin))  
          {  
                      pBegin++;  
                  }  
          while (func(*pEnd))  
          {  
                      pEnd--;  
                  }  
          int temp = *pBegin;  
          *pBegin = *pEnd;  
          *pEnd = temp;  
          pBegin++;  
          pEnd--;  
      }  
}  
    
    
void recordOddEven(int* pData, unsigned int length)  
{  
    if (pData == NULL || length == 0)  
        return;  
    record(pData, length, isEven);  
}  
      
        
          
bool isEven(int n)  
{  
  return (n & 1) == 0;  
}  


int main()  
{  
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8};  
    int len = 8;  
    recordOddEven(arr, len);  
    for (int i = 0; i < 8; i++)  
        cout<<arr[i]<<" ";  
    cout<<endl;  
} 

#if 0
public static void change(int[] arr) {
    //创建一个新数组,长度与原数组相同
    int[] newArr = new int[arr.length];
    //定义两个变量,从数组的头开始,和从数组的尾开始
    int index1 = 0;
    int index2 = arr.length - 1;
    //遍历传入的数组
    for (int i = 0; i < arr.length; i++) {
            if (arr[i] % 2 != 0) {
            //如果该元素是奇数,从数组左侧开始存
                    newArr[index1++] = arr[i];
            }else {
            //如果该元素是偶数,从新数组右侧开始存
                    newArr[index2--] = arr[i];
            }
    }
   
    //将新数组中的元素复制到传入的数组中
    for (int i = 0; i < newArr.length; i++) {
            arr[i] = newArr[i];
    }
}

#endif 
