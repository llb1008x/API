/*   创建一个记录所有的班级信息的xml文档
创建一个xml文档显得很简单，其流程如下：
1用xmlNewDoc函数创建一个文档指针doc；
2用xmlNewNode函数创建一个节点指针root_node；
3用xmlDocSetRootElement将root_node配置为doc的根结点；
4给root_node添加一系列的子节点，并配置子节点的内容和属性；
5用xmlSaveFile将xml文档存入文档；
6用xmlFreeDoc函数关闭文档指针，并清除本文档中任何节点动态申请的内存。
注意，有多种方式能够添加子节点：第一是用xmlNewTextChild直接添加一个文本子节点；
第二是先创建新节点，然后用xmlAddChild将新节点加入上层节点。
*/

#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "iconv.h"
#include <unistd.h>
#include <string.h>
//代码转换:从一种编码转为另一种编码   
int code_convert(char* from_charset, char* to_charset, char* inbuf,int inlen, char* outbuf, int outlen)
{
    iconv_t cd;
    char** pin = &inbuf;   
    char** pout = &outbuf;
    cd = iconv_open(to_charset,from_charset);   
    if(cd == 0)
       return -1;
    memset(outbuf,0,outlen);   
    if(iconv(cd,(const char**)pin,(unsigned int *)&inlen,pout,(unsigned int*)&outlen)== -1)
       return -1;   
    iconv_close(cd);
    return 0;   
}
//UNICODE码转为GB2312码   
//成功则返回一个动态分配的char*变量，需要在使用完毕后手动free，失败返回NULL
char* u2g(char *inbuf)   
{
    int nOutLen = 2 * strlen(inbuf) - 1;
    char* szOut = (char*)malloc(nOutLen);
    if (-1 == code_convert("utf-8","gb2312",inbuf,strlen(inbuf),szOut,nOutLen))
    {
       free(szOut);
       szOut = NULL;
    }
    return szOut;
}   
//GB2312码转为UNICODE码   
//成功则返回一个动态分配的char*变量，需要在使用完毕后手动free，失败返回NULL
char* g2u(char *inbuf)   
{
    int nOutLen = 2 * strlen(inbuf) - 1;
    char* szOut = (char*)malloc(nOutLen);
    if (-1 == code_convert("gb2312","utf-8",inbuf,strlen(inbuf),szOut,nOutLen))
    {
       free(szOut);
       szOut = NULL;
    }
    return szOut;
} 

int make_class_xml(int class_num, char**class_name, char *xml_name){
	//定义文档和节点指针
    xmlDocPtr doc = xmlNewDoc(BAD_CAST"1.0");//指定版本为1.0
    xmlNodePtr root_node = xmlNewNode(NULL,BAD_CAST"rfid_class");//rfid_class 节点
    //配置根节点
    xmlDocSetRootElement(doc,root_node);//将 rfid_class 节点即 root_node 指定为根节点
    //在根节点中直接创建子节点
	int i=0;
	for(i;i<class_num;i++){
		//char *str=g2u(class_name[i]);//GB2312
		xmlNodePtr node = xmlNewNode(NULL, BAD_CAST "class");
		xmlAddChild(root_node,node);
		xmlNewTextChild(node, NULL, BAD_CAST "class_name", BAD_CAST(class_name[i]));
	}
	int nRel = xmlSaveFormatFileEnc(xml_name, doc, "utf-8",1);
    //int nRel = xmlSaveFile(xml_name,doc);//指定文件存储的名字
    if (nRel != -1)
    {
       printf("一个xml文档被创建\n");   
	}
    //释放文档内节点动态申请的内存
    xmlFreeDoc(doc);
	return 1;
}
int main()
{
	char *xml_name="cn_class_xml.xml";//生成的文件名字
	int class_num = 3;//有几个班级
	char *class_name[3]={
					"鏈変汉",
					"3.16",
					"4.16"
					};//每个班级的名字
	make_class_xml(class_num,class_name,xml_name);
    return 1;
}
