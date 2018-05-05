#include "../pthread/arp_pthread.h"
#include "../source/get_interface.h"
#include "../project/types.h"




int flag =0;
//函数功能：计算头部校验和
unsigned short checksum(unsigned short *buf, int nword)
{
	unsigned long sum;	
	for(sum = 0; nword > 0; nword--)	
	{		
		sum += htons(*buf);		
		buf++;	
	}	
	sum = (sum>>16) + (sum&0xffff);	
	sum += (sum>>16);	
	return ~sum;
}
//线程
//函数功能：数据包的接收与转送
void *get_arp_answer(void *arg)
{	
	ROU *router = (ROU *)arg;
	router->router_arp_head = NULL;
	//flag = 0;
	//if( 0 == flag)
	//{
		while(1)
		{
			unsigned char des_net_ip[16] = "";//网段ip
			unsigned char des_ip[16] = "";//目标ip
			unsigned char src_mac[20] = "";//src_mac
			unsigned char src_ip[16] = "";//src_ip
			unsigned char answer[1530] = "";//数据包
			unsigned char src_port[20] = "";//源端口
			unsigned char agreement[20] = "";//协议类型
			unsigned char filt_str[50] = "";//过滤信息
			int len;

			ARP_LINK *p=NULL;
			//接收
			len = recvfrom(router->sockfd, answer, sizeof(answer), 0, NULL, NULL);//接收数据包	
			
			router->router_arp_new = (ARP_LINK*)malloc(sizeof(ARP_LINK));//申请链表空间
			//获取源MAC地址
			sprintf(src_mac,"%02x:%02x:%02x:%02x:%02x:%02x",answer[6],answer[7],answer[8],answer[9],answer[10],answer[11]);
			
			//Mac过滤
			bzero(filt_str,sizeof(filt_str));
			sprintf(filt_str,"filt_mac:%s",src_mac);
			if(filt_link_search(router->filt_link_head, filt_str) ==1)//查找是否存在过滤信息
			{
				continue;//终止当次循环：做到不理会此mac的目的
			}
					
			memcpy(router->router_arp_new->mac,src_mac,18);	//将源mac放入一个新节点上	
			//ARP				
			if((answer[12] == 0x08) && (answer[13] == 0x06))
			{
				//将源ip放入一个新节点上
				sprintf(router->router_arp_new->ip,"%d.%d.%d.%d",answer[28],answer[29],answer[30],answer[31]);
				link_insert(&router->router_arp_head,router->router_arp_new);//将此节点插入到链表中	
			}
			//IP
			if((answer[12]==0x08) && (answer[13]==0x00))
			{	
				
				sprintf(des_net_ip,"%d.%d.%d",answer[30],answer[31],answer[32]);//目的网段
				sprintf(des_ip,"%d.%d.%d.%d",answer[30],answer[31],answer[32],answer[33]);//目的IP	
				sprintf(src_ip,"%d.%d.%d.%d",answer[26],answer[27],answer[28],answer[29]);//源IP	
			
				//源IP过滤
				bzero(filt_str,sizeof(filt_str));
				sprintf(filt_str,"filt_ip:%s",src_ip);
				if(filt_link_search(router->filt_link_head, filt_str) ==1)//查找是否存在过滤信息
				{
					continue;//终止当次循环：做到不理会此ip的目的
				}
			
			
			//协议类型过滤
			    //获取当次协议类型
				if(answer[23]==1)
				{
					strcpy(agreement,"ICMP");
				}
				else if(answer[23]==2)
				{
					strcpy(agreement,"IGMP");
				}
				else if(answer[23]==6)
				{
					strcpy(agreement,"TCP");
				}
				else if(answer[23]==17)
				{
					strcpy(agreement,"UDP");
				}
				else if(answer[23]==41)
				{
					strcpy(agreement,"IPv6");
				}
				bzero(filt_str,sizeof(filt_str));
				sprintf(filt_str,"filt_por:%s",agreement);
				if(filt_link_search(router->filt_link_head, filt_str) ==1)//查找是否存在过滤信息
				{
					continue;//终止当次循环
				}
			//源端口号过滤
			    //获取端口号信息
				sprintf(src_port,"%d",(answer[34]*256+answer[35]));
				bzero(filt_str,sizeof(filt_str));
				sprintf(filt_str,"filt_port:%s",src_port);
				if(filt_link_search(router->filt_link_head, filt_str) ==1)//查找是否存在过滤信息
				{
					continue;//终止当次循环 不理会此端口发的信息
				}
				
				struct ifreq ifr;
				bzero(&ifr,sizeof(ifr));
				int num=0;
				for(num=0;num<interface_num;num++)
				{
					char eth[20]="";
					sprintf(eth,"%d.%d.%d",
							net_interface[num].ip[0]&net_interface[num].netmask[0],
							net_interface[num].ip[1]&net_interface[num].netmask[1],
							net_interface[num].ip[2]&net_interface[num].netmask[2]
							);//获取接口网段并组包
			
					if(strcmp(des_net_ip,eth)==0)//比较网段，确定接口
					{
						strncpy(ifr.ifr_name,net_interface[num].name,IFNAMSIZ);//指定网络接口名称
						break;
					}
				}
				if(num != 1&&num != 2)//不理会本地回环的情况
				{
					continue;
				}
				//将网络接口赋值给原始套接字地址结构
				if(ioctl(router->sockfd,SIOCGIFINDEX,(char *)&ifr)==-1)
				{
					perror("ioctl");//失败打印错误信息 
					close(router->sockfd);
					exit(-1);
				}
				
				struct sockaddr_ll sll;//原始套接字地址结构
				bzero(&sll,sizeof(sll));
				sll.sll_ifindex = ifr.ifr_ifindex;

				//查看有无目的ip对应的mac地址
				p = link_search_ip(router->router_arp_head,des_ip);
				if(p==NULL)//没有目的mac,全网段广播
				{	
					//广播包打包
					unsigned char send_buf[] = 
					{
					//--------------组MAC--------14------
					0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 		//目的mac	6	
					
					net_interface[num].mac[0],net_interface[num].mac[1],
					net_interface[num].mac[2],net_interface[num].mac[3],	//源Mac	  6		
					net_interface[num].mac[4],net_interface[num].mac[5],
					
					0x08, 0x06,								//ARP协议类型   2
					
					//--------------组ARP--------28-----
					0x00, 0x01, 0x08, 0x00,					//硬件类型1  2  协议类型0x0800(IP)  2	
					0x06, 0x04, 0x00, 0x01,					//硬件地址长度1、协议地址长度1  op:(1：arp请求＿：arp应答)  2
					
					net_interface[num].mac[0],net_interface[num].mac[1],
					net_interface[num].mac[2],net_interface[num].mac[3],	//源Mac	  6		
					net_interface[num].mac[4],net_interface[num].mac[5],

					net_interface[num].ip[0],net_interface[num].ip[1],					
					net_interface[num].ip[2],net_interface[num].ip[3],  	//源IP 		 6			
					
					0x00, 0x00, 0x00, 0x00, 0x00, 0x00,			//目的Mac   6
					
					answer[30],answer[31],answer[32],answer[33]	//目的IP  4							
					};
					sendto(router->sockfd,send_buf,42,0,(struct sockaddr *)&sll,sizeof(sll));
					
				}
				else //有目的mac的时候，转发数据
				{
					int i=0;
					for(i=0;i<6;i++)
					{
						answer[6+i] = net_interface[num].mac[i];//当前接口下的Mac  改为源MAC
					}
					
					//将ip对应的mac组包 变成目的mac
					sscanf(p->mac,"%x:%x:%x:%x:%x:%x",//截取字符串
							(unsigned int *)&answer[0],
							(unsigned int *)&answer[1],
							(unsigned int *)&answer[2],
							(unsigned int *)&answer[3],
							(unsigned int *)&answer[4],
							(unsigned int *)&answer[5]
							);
					
					answer[22] = answer[22]-1;//每次经过路由器生存时间TTL就减去1
					//头部校验和
					answer[24]=0;			
					answer[25]=0;			
					int check_sum = checksum((unsigned short *)(answer+14), 20/2 );
					answer[24]=check_sum>>8&0xff;
					answer[25]=check_sum&0xff;
					//组包发送
					sendto(router->sockfd,answer,len,0,(struct sockaddr *)&sll,sizeof(sll));
				}		
			}
			else
			{
				continue ;
			}
		}
	//}
	return NULL;
}

