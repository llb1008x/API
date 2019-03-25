//=========================================================================
//	文件名：	main.c
//  功能描述：	写一个函数，在链表中按学号查找该结点。
//	维护记录:	2008-4-18 v1.0		by xdw
//=========================================================================
typedef struct stu
{
	int num;
	int age;
	struct stu *next;
}TYPE;


TYPE *search (TYPE *head,int n)				//在一个已建立的链表中查找某一节点
{
	TYPE *p;
	p = head;								//导入已存在链表的头文件
	while( (p->num!=n) && (p->next!=NULL ))	//判断是否找到要找的数据，判断是否到链表尾。
	{
		p=p->next; 							//不是要找的结点后移一步
	}
		
	if( p->num == n )						//如果找到节点
	{
		return p;							//返回相应指针地址
	}
		
	if( p->num!=n && p->next==NULL )		//到链表尾则打印数据未找到
	{
		printf ("Node %d has not been found!\n",n);
	}
}
