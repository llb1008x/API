/*   ����xml�ĵ�
��������Ļ���������һ��xml�ĵ��Եúܼ򵥣����������£�
1��xmlNewDoc��������һ���ĵ�ָ��doc��
2��xmlNewNode��������һ���ڵ�ָ��root_node��
3��xmlDocSetRootElement��root_node����Ϊdoc�ĸ���㣻
4��root_node���һϵ�е��ӽڵ㣬�������ӽڵ�����ݺ����ԣ�
5��xmlSaveFile��xml�ĵ������ĵ���
6��xmlFreeDoc�����ر��ĵ�ָ�룬��������ĵ����κνڵ㶯̬������ڴ档
ע�⣬�ж��ַ�ʽ�ܹ�����ӽڵ㣺��һ����xmlNewTextChildֱ�����һ���ı��ӽڵ㣻
�ڶ����ȴ����½ڵ㣬Ȼ����xmlAddChild���½ڵ�����ϲ�ڵ㡣
Դ�����ĵ���CreateXmlFile.cpp�����£�*/

#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
int main()
{
    //�����ĵ��ͽڵ�ָ��
    xmlDocPtr doc = xmlNewDoc(BAD_CAST"1.0");//ָ���汾Ϊ1.0
    xmlNodePtr root_node = xmlNewNode(NULL,BAD_CAST"root");//����һ���ڵ�root��root_node��ʾ
    //���ø��ڵ�
    xmlDocSetRootElement(doc,root_node);//��root�ڵ㼴root_nodeָ��Ϊ���ڵ�
    //�ڸ��ڵ���ֱ�Ӵ����ӽڵ�
    xmlNewTextChild(root_node, NULL, BAD_CAST "newNode1", BAD_CAST "newNode1 content");
    xmlNewTextChild(root_node, NULL, BAD_CAST "newNode2", BAD_CAST "newNode2 content");
    xmlNewTextChild(root_node, NULL, BAD_CAST "newNode3", BAD_CAST "newNode3 content");
    //����һ���ڵ㣬���������ݺ����ԣ�Ȼ���������
    xmlNodePtr node = xmlNewNode(NULL,BAD_CAST"node2");
    xmlNodePtr content = xmlNewText(BAD_CAST"NODE CONTENT");
    xmlAddChild(root_node,node);//���뵽���ڵ���
    xmlAddChild(node,content);//�����֣�content�����뵽node��node2���ڵ���
    xmlNewProp(node,BAD_CAST"attribute",BAD_CAST "yes");//�������
    //����һ�����Ӻ����ӽڵ�
    node = xmlNewNode(NULL, BAD_CAST "son");
    xmlAddChild(root_node,node);
    xmlNodePtr grandson = xmlNewNode(NULL, BAD_CAST "grandson");
    xmlAddChild(node,grandson);
    xmlAddChild(grandson, xmlNewText(BAD_CAST "This is a grandson node"));//��������ӵ��ڵ��� ����xmlNewTextChild()
    //�洢xml�ĵ�
    int nRel = xmlSaveFile("CreatedXml.xml",doc);//ָ���ļ��洢������
    if (nRel != -1)
    {
       printf("һ��xml�ĵ�������\n");   
	}
    //�ͷ��ĵ��ڽڵ㶯̬������ڴ�
    xmlFreeDoc(doc);
    return 1;
}
/*
����һ��xml�ĵ�CreatedXml.xml���򿪺�������ʾ��
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