#include "arp_link.h"
#include "ip_link.h"
#include "ip_pthread.h"
#include "key_pthread.h"
#include "get_interface.h"

/******************************************************************
函  数：void print_help(void)
功	能:	打印帮助信息
参	数:	无
返回值: 无
*******************************************************************/
void print_help(void)
{
	printf("*************************************\n");
	printf("*      help:     打印帮助信息        *\n");
	printf("                                      \n");
	printf("*1      arp:     查看路由器的arp表   *\n");
	printf("*2 ifconfig:     查看路由器网卡信息  *\n");
	printf("*3 firewall:     设置防火墙          *\n");
	printf("*4   lsfire:     查看防火墙          *\n");
	printf("*5     exit:     退出                *\n");
	printf("*************************************\n");
}


/******************************************************************
函  数：void ifconfig()
功	能:	显示路由器网卡信息
参	数:	无
返回值: 无
*******************************************************************/
void ifconfig()
{
	int i;
	int interface_num = get_interface_num();    //获取网卡数目
	printf("路由器网卡信息：\n");
	for(i=0;i<interface_num;i++)
	{
		printf("*******************************\n");
		printf("   name:  %s  \n",net_interface[i].name);
		printf("    mac:  %02x:%02x:%02x:%02x:%02x:%02x  \n",net_interface[i].mac[0],
				net_interface[i].mac[1],net_interface[i].mac[2],
				net_interface[i].mac[3],net_interface[i].mac[4],
		        net_interface[i].mac[5]);
		
		printf("     ip:  %d.%d.%d.%d  \n",net_interface[i].ip[0],
				net_interface[i].ip[1],	net_interface[i].ip[2],
				net_interface[i].ip[3]);
		
		printf("netmask:  %d.%d.%d.%d  \n",net_interface[i].netmask[0],
		        net_interface[i].netmask[1],net_interface[i].netmask[2],
				net_interface[i].netmask[3]);
		printf("*******************************\n");
		
	}

}

/******************************************************************
函  数：void filt_ip( char *ip_buf )
功	能: 添加ip地址
参	数:	存放IP地址的数组的指针
返回值: 无
*******************************************************************/
void filt_ip(char *ip_buf)
{
	IP  *p_new;
	unsigned char ip[4]="";
	inet_pton(AF_INET, ip_buf, &ip);
	if( NULL == ip_search_ip(ip_head, ip))                //找不到则添加
	{
		p_new = (IP*)malloc(sizeof(IP));                  //申请节点空间
		memcpy(p_new->ip, ip, 4);			              //节点的IP地址赋值
		ip_head = ip_link_creat(ip_head, p_new);		  //插入链表
		save_file( ip_head );
	}
}


/******************************************************************
函  数：void filt_mac( char *mac_buf )
功	能: 添加mac地址
参	数:	存放MAC地址的数组的指针
返回值: 无
*******************************************************************/
void filt_mac(char *mac_buf)
{
	IP *p_new;
	
	if( NULL == ip_search_mac(ip_head, (unsigned char *)mac_buf))
	{
		p_new = (IP*)malloc(sizeof(IP));                  //申请节点空间
		memcpy(p_new->mac, mac_buf, strlen(mac_buf));	  //节点的mac地址赋值
		ip_head = ip_link_creat(ip_head, p_new);		  //插入链表
		save_file(ip_head);
	}
}

/******************************************************************
函  数：void del_ip( char *ip_buf )
功	能: 删除ip地址
参	数:	存放IP地址的数组的指针
返回值: 无
*******************************************************************/
void del_ip( char *ip_buf )
{
	unsigned char ip[4]="";
	inet_pton(AF_INET, ip_buf, &ip);            //点分十进制数转化成32位无符号整数
	ip_link_del_ip(ip_head, ip);
	save_file( ip_head );	
}


/******************************************************************
函  数：void del_mac( char *mac_buf )
功	能: 删除mac地址
参	数:	存放mac地址的数组的指针
返回值: 无
*******************************************************************/
void del_mac( char *mac_buf )
{
	ip_link_del_mac(&ip_head, (unsigned char *)mac_buf);
	save_file( ip_head );	
}

/******************************************************************
函  数：void firewall()
功	能: 设置防火墙
参	数:	无
返回值: 无
*******************************************************************/
void firewall()
{
	printf("please input:\n");
	printf("*************************************\n");
	printf("*3.1 filt_ip      添加过滤IP        *\n");
	printf("*3.2 del_ip       删除过滤IP        *\n");
    printf("*3.3 filt_mac     添加过滤MAC       *\n");
	printf("*3.4 del_mac      删除过滤MAC       *\n");
	printf("*3.5 back         返回上层界面      *\n");
	printf("*3.6 exit         退出              *\n");
	printf("*************************************\n");
	
	char buf[100] = "";
	char cmd[50]="";
	char ip_buf[17]="";
	char mac_buf[19]="";
	while(1)
	{
		fgets(cmd,sizeof(buf),stdin);                   //获取命令
		buf[strlen(cmd)-1]='\0';   
		
		if(0 == memcmp(cmd,"3.1",3))                    //添加IP
		{	
			printf("请输入需要过滤的IP: ");             
			fflush(stdout);
			fgets(ip_buf,sizeof(ip_buf),stdin);         //获取ip
			ip_buf[strlen(ip_buf)-1]='\0'; 		
			filt_ip(ip_buf);
		}
		else if(0 == memcmp(cmd,"3.2",3))               //删除IP
		{
			printf("请输入需要删除的IP: ");
			fflush(stdout);
			fgets(ip_buf,sizeof(ip_buf),stdin);         //获取ip
			ip_buf[strlen(ip_buf)-1]='\0';  
			del_ip(ip_buf);
		}
		else if(0 == memcmp(cmd,"3.3",3))               //添加mac
		{
			printf("请输入需要添加的mac: ");
			fflush(stdout);
			fgets(mac_buf,sizeof(mac_buf),stdin);       //获取mac
			mac_buf[strlen(mac_buf)-1]='\0';  
			filt_mac(mac_buf);
		}
		else if(0 == memcmp(cmd,"3.4",3))               //删除mac
		{
			printf("请输入需要删除的mac: ");
			fflush(stdout);
			fgets(mac_buf,sizeof(mac_buf),stdin);       //获取mac
			mac_buf[strlen(mac_buf)-1]='\0';  
			del_mac(mac_buf);
		}
		else if(0 == memcmp(cmd,"3.5",3))               //返回上层界面
		{
			printf("已返回上层界面,请输入help查看帮助信息\n");
			break;
		}
		else if(0 == memcmp(cmd,"3.6",3))               //退出
		{
			printf("谢谢使用！\n");
			exit(-1);
		}
		else
		{
			printf("已返回上层界面,请输入help查看帮助信息\n");
			break;
		}
	}
}

/******************************************************************
函  数：void exit_all(void)
功	能: 退出界面
参	数:	无
返回值: 无
*******************************************************************/
void exit_all(void)
{
	printf("谢谢使用！\n");
	exit(-1);
}

/******************************************************************
函  数：void *key_pthread(void *arg)
功	能: 键盘输入命令
参	数:	void *arg
返回值: void *
*******************************************************************/
void *key_pthread(void *arg)
{
	printf("输入help获取帮助\n");
	
   char cmd_buf[100]="";     //定义字符串，防止内存泄露，导致刷屏
   
   while(1)
   {
		fgets(cmd_buf, sizeof(cmd_buf), stdin); 
		//输入命令
		if(0 == memcmp(cmd_buf, "help", 4))                           //帮助信息
			print_help();
		else if(0 == memcmp(cmd_buf,"1",strlen(cmd_buf)-1))           //打印路由器ARP缓存表信息
			arp_print(arp_head);
		else if(0 == memcmp(cmd_buf,"2",strlen(cmd_buf)-1))           //打印路由器信息
			ifconfig();	
		else if(0 == memcmp(cmd_buf,"3",strlen(cmd_buf)-1))		      //设置防火墙
		    firewall();
		else if(0 == memcmp(cmd_buf,"4",strlen(cmd_buf)-1))           //查看防火墙
		    ip_print(ip_head);
		else if(0 == memcmp(cmd_buf,"5",strlen(cmd_buf)-1))           //退出
		    exit_all();
		else
		    printf("error\n");
   }
   return NULL;
}
