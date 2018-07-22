#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

int main(int argc,char** argv)
{
	xmlDocPtr doc=NULL;
	xmlNodePtr cur=NULL;
	char* name=NULL;
	char* value=NULL;
	xmlKeepBlanksDefault(0);//ȥ���ո�
	doc=xmlParseFile("show_class.xml");//��һ���ļ�������Dom��
	cur=xmlDocGetRootElement(doc);//��ȡ���ڵ�
	cur=cur->xmlChildrenNode;//�õ���һ���ӽڵ�
	while(cur !=NULL){
		name=(char*)(cur->name);
		value=(char*)xmlNodeGetContent(cur);
		printf("name is:%s value is:%s\n",name,value);
		xmlFree(value);//ע�⣬����Ҫfree��
		cur=cur->next; //��һ���ֽڵ�
	}
	xmlFreeDoc(doc);//�ͷ�xml������������Դ
	xmlCleanupParser();
	return 0;
}

/*
XML��
<?xml version="1.0" encoding="utf-8"?>
<web_rfid>
	<class>
		<class_name>��ʦ</class_name>
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

�����
name is:class value is:��ʦ
name is:class value is:2.26
name is:class value is:3.21
name is:class value is:4.12
name is:class value is:5.12
name is:class value is:6.5
*/







