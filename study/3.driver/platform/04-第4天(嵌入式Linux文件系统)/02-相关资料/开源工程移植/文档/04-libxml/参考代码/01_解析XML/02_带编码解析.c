/*********************************************************
����һ��xml�ĵ�������ȡ����Ҫ����Ϣ������ڵ��а��������֣���ĳ���ڵ�����ԣ����������£�
1��xmlReadFile��������һ���ĵ�ָ��doc��
2��xmlDocGetRootElement�����õ����ڵ�curNode��
3curNode->xmlChildrenNode���Ǹ��ڵ���ӽڵ㼯�ϣ�
4��ѯ�ӽڵ㼯�ϣ��ҵ�����Ľڵ㣬��xmlNodeGetContentȡ�������ݣ�
5��xmlHasProp���Һ���ĳ�����ԵĽڵ㣻
6ȡ���ýڵ�����Լ��ϣ���xmlGetPropȡ��������ֵ��
7��xmlFreeDoc�����ر��ĵ�ָ�룬��������ĵ����κνڵ㶯̬������ڴ档
ע�⣺�ڵ��б��ָ����Ȼ��xmlNodePtr�������б��ָ��Ҳ��xmlAttrPtr��
	��û��xmlNodeList��xmlAttrList���������͡�
	�����б��ʱ��ʹ�����ǵ�next��prev����ָ����������ѯ��
	ֻ����Xpath����xmlNodeSet�������ͣ���ʹ�÷���ǰ���ѽ����ˡ�
*********************************************************************/

#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h> 
int main(int argc, char* argv[])
{
    xmlDocPtr doc;           //��������ĵ�ָ��
    xmlNodePtr curNode;      //������ָ��(����Ҫ��Ϊ���ڸ��������ƶ�) 
    xmlChar *szKey;          //��ʱ�ַ�������
    char *szDocName="CreatedXml.xml";
  
    doc = xmlReadFile(szDocName,"UTF-8",XML_PARSE_RECOVER); //�����ĵ�
    //�������ĵ��Ƿ�ɹ������粻�ɹ���libxml��ָһ��ע��Ĵ���ֹͣ��
    //һ�����������ǲ��ʵ��ı��롣XML��׼�ĵ�������UTF-8��UTF-16�⻹�����������뱣�档
    //�����ĵ���������libxml���Զ���Ϊ��ת����UTF-8���������XML������Ϣ������XML��׼��.
    if (NULL == doc) 
    {  
       printf("Document not parsed successfully");     
       return -1; 
    } 
    curNode = xmlDocGetRootElement(doc); //ȷ���ĵ���Ԫ��
    /*���ȷ�ϵ�ǰ�ĵ��а�������*/ 
    if (NULL == curNode)
    { 
       printf("empty document"); 
       xmlFreeDoc(doc); 
       return -1; 
    } 
    /*����������У�������Ҫȷ���ĵ�����ȷ�����͡���root���������ʾ����ʹ���ĵ��ĸ����͡�*/
    if (xmlStrcmp(curNode->name, BAD_CAST "root")) 
    {
	   printf("document of the wrong type, root node != root"); 
       xmlFreeDoc(doc); 
       return -1; 
    } 
    curNode = curNode->xmlChildrenNode;
    xmlNodePtr propNodePtr = curNode;
    while(curNode != NULL) 
    {
       //ȡ���ڵ��е�����
       if ((!xmlStrcmp(curNode->name, (const xmlChar *)"newNode1"))) 
       {
           szKey = xmlNodeGetContent(curNode);
           printf("newNode1: %s\n",szKey); 
           xmlFree(szKey); 
       } 
       //���Ҵ�������attribute�Ľڵ�
       if (xmlHasProp(curNode,BAD_CAST "attribute"))
       {
           propNodePtr = curNode;
       }
       curNode = curNode->next; 
    }
    //��������
    xmlAttrPtr attrPtr = propNodePtr->properties;
    while (attrPtr != NULL)
    {
       if (!xmlStrcmp(attrPtr->name, BAD_CAST "attribute"))
       {
           xmlChar* szAttr = xmlGetProp(propNodePtr,BAD_CAST "attribute");
		   printf("attribute: %s\n",szAttr); 
           xmlFree(szAttr);
       }
       attrPtr = attrPtr->next;
    }
    xmlFreeDoc(doc);
    return 0;
}
/*
CreatedXml.xml
�۲�Դ����ɷ��֣��κ��Բ�ѯ��ʽ�õ���xmlChar*�ַ���������ʹ��xmlFree�����ֶ��ͷš�
���������ڴ�й©��
�����
newNode1: newNode1 content
attribute: yes
*/