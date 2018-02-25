/*   创建xml文档
有了上面的基础，创建一个xml文档显得很简单，其流程如下：
1用xmlNewDoc函数创建一个文档指针doc；
2用xmlNewNode函数创建一个节点指针root_node；
3用xmlDocSetRootElement将root_node配置为doc的根结点；
4给root_node添加一系列的子节点，并配置子节点的内容和属性；
5用xmlSaveFile将xml文档存入文档；
6用xmlFreeDoc函数关闭文档指针，并清除本文档中任何节点动态申请的内存。
注意，有多种方式能够添加子节点：第一是用xmlNewTextChild直接添加一个文本子节点；
第二是先创建新节点，然后用xmlAddChild将新节点加入上层节点。
源代码文档是CreateXmlFile.cpp，如下：*/

#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
int main()
{
    //定义文档和节点指针
    xmlDocPtr doc = xmlNewDoc(BAD_CAST"1.0");//指定版本为1.0
    xmlNodePtr root_node = xmlNewNode(NULL,BAD_CAST"root");//创建一个节点root用root_node标示
    //配置根节点
    xmlDocSetRootElement(doc,root_node);//将root节点即root_node指定为根节点
    //在根节点中直接创建子节点
    xmlNewTextChild(root_node, NULL, BAD_CAST "newNode1", BAD_CAST "newNode1 content");
    xmlNewTextChild(root_node, NULL, BAD_CAST "newNode2", BAD_CAST "newNode2 content");
    xmlNewTextChild(root_node, NULL, BAD_CAST "newNode3", BAD_CAST "newNode3 content");
    //创建一个节点，配置其内容和属性，然后加入根结点
    xmlNodePtr node = xmlNewNode(NULL,BAD_CAST"node2");
    xmlNodePtr content = xmlNewText(BAD_CAST"NODE CONTENT");
    xmlAddChild(root_node,node);//加入到根节点中
    xmlAddChild(node,content);//将文字（content）加入到node（node2）节点中
    xmlNewProp(node,BAD_CAST"attribute",BAD_CAST "yes");//添加属性
    //创建一个儿子和孙子节点
    node = xmlNewNode(NULL, BAD_CAST "son");
    xmlAddChild(root_node,node);
    xmlNodePtr grandson = xmlNewNode(NULL, BAD_CAST "grandson");
    xmlAddChild(node,grandson);
    xmlAddChild(grandson, xmlNewText(BAD_CAST "This is a grandson node"));//将文字添加到节点中 类似xmlNewTextChild()
    //存储xml文档
    int nRel = xmlSaveFile("CreatedXml.xml",doc);//指定文件存储的名字
    if (nRel != -1)
    {
       printf("一个xml文档被创建\n");   
	}
    //释放文档内节点动态申请的内存
    xmlFreeDoc(doc);
    return 1;
}
/*
生成一个xml文档CreatedXml.xml，打开后如下所示：
<?xml version="1.0"?>
<root>
	<newNode1>newNode1 content</newNode1>
	<newNode2>newNode2 content</newNode2>
	<newNode3>newNode3 content</newNode3>
	<node2 attribute="yes">NODE CONTENT</node2>
	<son>
		<grandson>This is a grandson node</grandson>
	</son>
</root>
*/