#ifndef FIREWALL_PTHREAD_H
#define FIREWALL_PTHREAD_H

/******************************************************************
功	能:	键盘输入处理线程
参	数:	无
返回值: 无
*******************************************************************/
extern void *firewall_pthread(void *arg);

/******************************************************************
函数功能:	显示帮助信息
参	  数:	无
返 回 值: 	无
*******************************************************************/
extern void printf_help();

/******************************************************************
函数功能:	打印arp缓存表信息
参	  数:	无
返 回 值: 	无
*******************************************************************/
extern void showarp();

/******************************************************************
函数功能:	显示网卡信息
参	  数:	无
返 回 值:   无
*******************************************************************/
extern void ifconfig();

/******************************************************************
函数功能:	设置防火墙
参	  数:	无
返 回 值:   无
*******************************************************************/
extern void firewall();

/******************************************************************
函数功能:	添加ip过滤条件
参	  数:	无
返 回 值:   无
*******************************************************************/
extern void add_ip();

/******************************************************************
函数功能:	删除IP过滤条件
参	  数:	无
返 回 值:   无
*******************************************************************/
extern void dele_ip();

/******************************************************************
函数功能:	添加mac过滤条件
参	  数:	无
返 回 值: 	无
*******************************************************************/
extern void add_mac();

/******************************************************************
函数功能:	删除mac过滤条件
参	  数:	无
返 回 值: 	无
*******************************************************************/
extern void dele_mac();

/******************************************************************
函数功能:	查看防火墙信息
参	  数:	无
返 回 值:	无
*******************************************************************/
extern void print_firewall();

/******************************************************************
函数功能:	保存防火墙信息到文件
参	  数:	无
返 回 值:	无
*******************************************************************/
extern void save_firewall_to_file();

/******************************************************************
函数功能:	退出程序
参    数:	无
返 回 值:   无
*******************************************************************/
extern void exit_route();

#endif