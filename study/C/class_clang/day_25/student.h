/*************************************************************************
	> File Name: student.h
	> Author: llb
	> Mail: llb1008x@126.com 
	> Created Time: 2017年06月15日 星期四 08时35分01秒
 ************************************************************************/

#ifndef __STUDENT_H__
#define __STUDENT_H__

const int STR_LEN=20;

typedef struct _student{

	char name[STR_LEN];
	int gender;
	int age;
}Student;

#endif
