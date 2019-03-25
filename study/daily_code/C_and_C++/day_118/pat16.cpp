/*
1003 我要通过！ （20 分）
“答案正确”是自动判题系统给出的最令人欢喜的回复。本题属于 PAT 的“答案正确”大派送 —— 只要读入的字符串满足下列条件，系统就输出“答案正确”，否则输出“答案错误”。

得到“答案正确”的条件是：

字符串中必须仅有 P、 A、 T这三种字符，不可以包含其它字符；
任意形如 xPATx 的字符串都可以获得“答案正确”，其中 x 或者是空字符串，或者是仅由字母 A 组成的字符串；
如果 aPbTc 是正确的，那么 aPbATca 也是正确的，其中 a、 b、 c 均或者是空字符串，或者是仅由字母 A 组成的字符串。
现在就请你为 PAT 写一个自动裁判程序，判定哪些字符串是可以获得“答案正确”的。

输入格式：
每个测试输入包含 1 个测试用例。第 1 行给出一个正整数 n (<10)，是需要检测的字符串个数。接下来每个字符串占一行，字符串长度不超过 100，且不包含空格。

输出格式：
每个字符串的检测结果占一行，如果该字符串可以获得“答案正确”，则输出 YES，否则输出 NO。

输入样例：
8
PAT
PAAT
AAPATAA
AAPAATAAAA
xPATx
PT
Whatever
APAAATAA
输出样例：
YES
YES
YES
YES
NO
NO
NO
NO

*/

// 我提交的显示段错误

#include <iostream>

using namespace std;

int main()
{
    int N,i=0,j=0;
    int count=0;
    //要输入字符串的个数
    scanf("%d",&N);
    if(N>=10 || N<=0)
        return -1;
    
    i=N;
    //存储输入的字符串
    char (*str)[10];
    while(N>0){
        scanf("%s",str[N]);
        N--;
    }
    
    //输出字符串
    while(i>0){
        //printf("%s\n",str[i]);
        //判读字符串
        while(str[i][j]!='\0'){
        
            if(count == 0){
                
                if(str[i][j]=='P')
                        count++;
                else if(str[i][j]!='A' /*|| str[i][j]!=' '*/)
                        break;
            }else if(count==1){
                
                if(str[i][j]=='A')
                        count++;
                  
            }else if(count==2){
                
                if(str[i][j]=='T')
                        count++;
                if(str[i][j]!='A' /* || str[i][j]!=' ' */)
                        break;        
       
            }
            
            j++;    
        }
        //printf("count->%d\n",count);
        if(count==3)
            printf("YES\n");
        else
            printf("NO\n");    
        count=0;    
        i--;
        j=0;
    }
    
    return 0;
}



/*  网上大神写的
#include <iostream>
#include <map>
using namespace std;
int main() {
    int n, p = 0, t = 0;
    string s;
    cin >> n;
    for(int i = 0; i < n; i++) {
        cin >> s;
        map<char, int> m;
        for(int j = 0; j < s.size(); j++) {
            m[s[j]]++;
            if (s[j] == 'P') p = j;
            if (s[j] == 'T') t = j;
        }
        if (m['P'] == 1 && m['A'] != 0 && m['T'] == 1 && m.size() == 3 && t-p != 1 && p * (t-p-1) == s.length()-t-1)
            printf("YES\n");
        else
            printf("NO\n");
    }
    return 0;
}


*/
