/*********************************************************
解析一个xml文档，从中取出想要的信息，例如节点中包含的文字，或某个节点的属性，其流程如下：
1用xmlReadFile函数读出一个文档指针doc；
2用xmlDocGetRootElement函数得到根节点curNode；
3curNode->xmlChildrenNode就是根节点的子节点集合；
4轮询子节点集合，找到所需的节点，用xmlNodeGetContent取出其内容；
5用xmlHasProp查找含有某个属性的节点；
6取出该节点的属性集合，用xmlGetProp取出其属性值；
7用xmlFreeDoc函数关闭文档指针，并清除本文档中任何节点动态申请的内存。
注意：节点列表的指针依然是xmlNodePtr，属性列表的指针也是xmlAttrPtr，
	并没有xmlNodeList或xmlAttrList这样的类型。
	看作列表的时候使用他们的next和prev链表指针来进行轮询。
	只有在Xpath中有xmlNodeSet这种类型，其使用方法前面已介绍了。
*********************************************************************/

#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h> 
int main(int argc, char* argv[])
{
    xmlDocPtr doc;           //定义解析文档指针
    xmlNodePtr curNode;      //定义结点指针(您需要他为了在各个结点间移动) 
    xmlChar *szKey;          //临时字符串变量
    char *szDocName="CreatedXml.xml";
  
    doc = xmlReadFile(szDocName,"UTF-8",XML_PARSE_RECOVER); //解析文档
    //检查解析文档是否成功，假如不成功，libxml将指一个注册的错误并停止。
    //一个常见错误是不适当的编码。XML标准文档除了用UTF-8或UTF-16外还可用其他编码保存。
    //假如文档是这样，libxml将自动地为您转换到UTF-8。更多关于XML编码信息包含在XML标准中.
    if (NULL == doc) 
    {  
       printf("Document not parsed successfully");     
       return -1; 
    } 
    curNode = xmlDocGetRootElement(doc); //确定文档根元素
    /*检查确认当前文档中包含内容*/ 
    if (NULL == curNode)
    { 
       printf("empty document"); 
       xmlFreeDoc(doc); 
       return -1; 
    } 
    /*在这个例子中，我们需要确认文档是正确的类型。“root”是在这个示例中使用文档的根类型。*/
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
       //取出节点中的内容
       if ((!xmlStrcmp(curNode->name, (const xmlChar *)"newNode1"))) 
       {
           szKey = xmlNodeGetContent(curNode);
           printf("newNode1: %s\n",szKey); 
           xmlFree(szKey); 
       } 
       //查找带有属性attribute的节点
       if (xmlHasProp(curNode,BAD_CAST "attribute"))
       {
           propNodePtr = curNode;
       }
       curNode = curNode->next; 
    }
    //查找属性
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
观察源代码可发现，任何以查询方式得到的xmlChar*字符串都必须使用xmlFree函数手动释放。
否则会造成内存泄漏。
结果：
newNode1: newNode1 content
attribute: yes
*/