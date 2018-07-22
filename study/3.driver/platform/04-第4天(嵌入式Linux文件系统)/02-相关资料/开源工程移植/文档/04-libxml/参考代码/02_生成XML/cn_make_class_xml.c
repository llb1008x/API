/*   ����һ����¼���еİ༶��Ϣ��xml�ĵ�
����һ��xml�ĵ��Եúܼ򵥣����������£�
1��xmlNewDoc��������һ���ĵ�ָ��doc��
2��xmlNewNode��������һ���ڵ�ָ��root_node��
3��xmlDocSetRootElement��root_node����Ϊdoc�ĸ���㣻
4��root_node���һϵ�е��ӽڵ㣬�������ӽڵ�����ݺ����ԣ�
5��xmlSaveFile��xml�ĵ������ĵ���
6��xmlFreeDoc�����ر��ĵ�ָ�룬��������ĵ����κνڵ㶯̬������ڴ档
ע�⣬�ж��ַ�ʽ�ܹ�����ӽڵ㣺��һ����xmlNewTextChildֱ�����һ���ı��ӽڵ㣻
�ڶ����ȴ����½ڵ㣬Ȼ����xmlAddChild���½ڵ�����ϲ�ڵ㡣
*/

#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "iconv.h"
#include <unistd.h>
#include <string.h>
//����ת��:��һ�ֱ���תΪ��һ�ֱ���   
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
//UNICODE��תΪGB2312��   
//�ɹ��򷵻�һ����̬�����char*��������Ҫ��ʹ����Ϻ��ֶ�free��ʧ�ܷ���NULL
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
//GB2312��תΪUNICODE��   
//�ɹ��򷵻�һ����̬�����char*��������Ҫ��ʹ����Ϻ��ֶ�free��ʧ�ܷ���NULL
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
	//�����ĵ��ͽڵ�ָ��
    xmlDocPtr doc = xmlNewDoc(BAD_CAST"1.0");//ָ���汾Ϊ1.0
    xmlNodePtr root_node = xmlNewNode(NULL,BAD_CAST"rfid_class");//rfid_class �ڵ�
    //���ø��ڵ�
    xmlDocSetRootElement(doc,root_node);//�� rfid_class �ڵ㼴 root_node ָ��Ϊ���ڵ�
    //�ڸ��ڵ���ֱ�Ӵ����ӽڵ�
	int i=0;
	for(i;i<class_num;i++){
		//char *str=g2u(class_name[i]);//GB2312
		xmlNodePtr node = xmlNewNode(NULL, BAD_CAST "class");
		xmlAddChild(root_node,node);
		xmlNewTextChild(node, NULL, BAD_CAST "class_name", BAD_CAST(class_name[i]));
	}
	int nRel = xmlSaveFormatFileEnc(xml_name, doc, "utf-8",1);
    //int nRel = xmlSaveFile(xml_name,doc);//ָ���ļ��洢������
    if (nRel != -1)
    {
       printf("һ��xml�ĵ�������\n");   
	}
    //�ͷ��ĵ��ڽڵ㶯̬������ڴ�
    xmlFreeDoc(doc);
	return 1;
}
int main()
{
	char *xml_name="cn_class_xml.xml";//���ɵ��ļ�����
	int class_num = 3;//�м����༶
	char *class_name[3]={
					"有人",
					"3.16",
					"4.16"
					};//ÿ���༶������
	make_class_xml(class_num,class_name,xml_name);
    return 1;
}
