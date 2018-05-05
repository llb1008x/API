#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

int main(int argc,char** argv)
{
	xmlDocPtr doc=NULL;
	xmlNodePtr cur=NULL;
	char* name=NULL;
	char* value=NULL;
	xmlKeepBlanksDefault(0);//去除空格
	doc=xmlParseFile("show_class.xml");//打开一个文件，创建Dom树
	cur=xmlDocGetRootElement(doc);//获取根节点
	cur=cur->xmlChildrenNode;//得到第一个子节点
	while(cur !=NULL){
		name=(char*)(cur->name);
		value=(char*)xmlNodeGetContent(cur);
		printf("name is:%s value is:%s\n",name,value);
		xmlFree(value);//注意，必须要free掉
		cur=cur->next; //下一个字节点
	}
	xmlFreeDoc(doc);//释放xml解析库所用资源
	xmlCleanupParser();
	return 0;
}

/*
XML：
<?xml version="1.0" encoding="utf-8"?>
<web_rfid>
	<class>
		<class_name>教师</class_name>
	</class>
	<class>
		<class_name>2.26</class_name>
	</class>
	<class>
		<class_name>3.21</class_name>
	</class>
	<class>
		<class_name>4.12</class_name>
	</class>
	<class>
		<class_name>5.12</class_name>
	</class>
	<class>
		<class_name>6.5</class_name>
	</class>
</web_rfid>

结果：
name is:class value is:教师
name is:class value is:2.26
name is:class value is:3.21
name is:class value is:4.12
name is:class value is:5.12
name is:class value is:6.5
*/







