/******************************************************************
			字符串匹配算法
******************************************************************/
#include<iostream>
#include<string>


using namespace std;
void compute_overlay(const string& pattern)
{
    const int pattern_length = pattern.size();
    int *overlay_function = new int[pattern_length];
    int index;
    overlay_function[0] = -1;
    for(int i=1;i<pattern_length;i++)
    {
        index = overlay_function[i-1];
        //store previous fail position k to index;
       
        while(index>=0 && pattern[i]!=pattern[index+1])
        {
            index = overlay_function[index];
        }
        if(pattern[i]==pattern[index+1])
        {
            overlay_function[i] = index + 1; 
        }
        else
        {
            overlay_function[i] = -1;
        }
    }
    for(int i=0;i<pattern_length;i++)
    {
        cout<<overlay_function[i]<<endl;
    }
    delete[] overlay_function;
}
int main()
{
    string pattern = "abaabcaba";
    compute_overlay(pattern);
    return 0;
}
