#ifndef IP_LINK_H
#define IP_LINK_H

typedef struct ip_link{
	unsigned char ip[4];	//ip��ַ�ĸ��ֽ�
	struct ip_link *next;
}IP_LINK;
extern IP_LINK *ip_head;
/******************************************************************
��	��:	IP_LINK *find_ip(IP_LINK *head, unsigned char *ip)
��	��:	����ip��������
��	��:	IP_LINK *head ip��������ͷ  IP_LINK* p ������ڵ�
����ֵ: IP_LINK *�ҵ��Ľڵ�
*******************************************************************/
extern IP_LINK *inner_ip_link(IP_LINK *ip_head,IP_LINK* p);
/******************************************************************
��	��:	void printf_ip_link(IP_LINK *head)
��	��:	��ӡip��������
��	��:	IP_LINK *head ip��������ͷ
����ֵ: ��
*******************************************************************/
extern void printf_ip_link(IP_LINK *ip_head);
/******************************************************************
��	��:	IP_LINK *find_ip(IP_LINK *head, unsigned char *ip)
��	��:	����ip��������
��	��:	IP_LINK *head ip��������ͷ  unsigned char *ip ������IP
����ֵ: IP_LINK *�ҵ��Ľڵ�
*******************************************************************/
extern IP_LINK *find_ip(IP_LINK *ip_head, unsigned char *ip);
/******************************************************************
��	��:	void init_ip_link()
��	��:	�ͷ�ip��������
��	��:	IP_LINK *head ip��������ͷ
����ֵ: ��
*******************************************************************/
extern void free_ip_link(IP_LINK *ip_head);
/******************************************************************
��	��:	IP_LINK *del_ip_for_link(IP_LINK *head, unsigned char *ip)
��	��:	ɾ��ip��������ڵ�
��	��:	IP_LINK *head ip��������ͷ  unsigned char *ip ��ɾ��IP
����ֵ: IP_LINK *head ip��������ͷ
*******************************************************************/
extern IP_LINK *del_ip_for_link(IP_LINK *head,unsigned char *ip);
/******************************************************************
��	��:	void init_ip_link()
��	��:	��ȡ�����ļ����ݵ�����
��	��:	��
����ֵ: ��
*******************************************************************/
extern void init_ip_link();
/******************************************************************
��	��:	void save_ip_link()
��	��:	�����������ݵ������ļ�
��	��:	��
����ֵ: ��
*******************************************************************/
extern void save_ip_link();

#endif

