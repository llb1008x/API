/*************************************************************************
	> File Name: test3.c
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年06月30日 星期五 15时08分05秒
	
	给定一组二维坐标，表示直角坐标系内的一个多边形的连续的顶点的坐标序列。计算能包围这个多边
	形的平行于坐标轴的最小矩形，输出它的左下角和右上角的坐标。


输入格式:

第一行是一个正整数n表示顶点的数量，第二行是n组整数，依次表示每个顶点坐标的x和y值。


输出格式：

四个整数，依次表示所计算的矩形的左下角的坐标的x、y值和右上角坐标的x、y值。输出最后带有回车换行。


输入样例：

5

1 1 1 4 3 7 4 4 4 1


输出样例：

1 1 4 7
 ************************************************************************/

#include <stdio.h>  
#define ARRAY_SIZE 100  
struct Point {  
    int x;  
    int y;  
};  
struct Point p[ARRAY_SIZE];  
    
int main(void) {  
    int i, n;  
    int min_x, max_x, min_y, max_y;  
    scanf("%d", &n);  
    for(i = 0; i < n; ++i)  
        scanf("%d%d", &p[i].x, &p[i].y);  
    min_x = max_x = p[0].x;  
    min_y = max_y = p[0].y;  
    for(i = 1; i < n; ++i) {  
        if(min_x > p[i].x)  
            min_x = p[i].x;  
        if(max_x < p[i].x)  
            max_x = p[i].x;  
        if(min_y > p[i].y)  
            min_y = p[i].y;  
        if(max_y < p[i].y)  
            max_y = p[i].y;  
    }  
    printf("%d %d %d %d\n", min_x, min_y, max_x, max_y);  
    
    return 0;  
}  
