/*************************************************************************
	> File Name: string.h
	> Author: llb
	> Mail: llb1008x@126.com
	> Created Time: 2017年08月10日 星期四 23时10分24秒
 ************************************************************************/

#ifndef _STRING_H
#define _STRING_H


//计算字符串的长度
extern int mystrlen(char *str);


//比较两个字符串的大小
extern int mystrcmp(char *str1,char *str2);


//比较两个字符串的前n个字节
extern int mystrncmp(char *str1,char *str2,int n);


//字符串拷贝函数
extern char *mystrcpy(char *dst,char *src);


//将第二个字符串的n个字节拷贝出来
extern char *mystrncpy(char *dst,char *src,int n);


//字符串连接函数，将第二个字符串连接到第一个上
extern char *mystrcat(char *dst,char *src);


//将字符串前n个字节拷贝到前面
extern char *mystrncat(char *dst,char *src,int n);


//字符匹配
extern char *mystrchr(char *src,char ch);


//字符串转
extern int myatoi(char *str);

#endif
