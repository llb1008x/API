#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<pthread.h>
#include <sys/socket.h>
#include <netpacket/packet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <net/ethernet.h>
#include <strings.h>
#include <libnet.h>
#include <unistd.h>         
#include "../pthread/arp_pthread.h"
#include "../source/ip_filter.h"
#include "../source/ip_link.h"
#include "../source/get_interface.h"

#include "../project/types.h"
#include "../pthread/udp_pthread.h"




void *set_arp_req(void * arg)//远程控制（UDP）
{
	ROU *router = (ROU *)arg;
		
	int sockfd_udp = socket(AF_INET,SOCK_DGRAM,0);
	if(sockfd_udp<0)
	{
		perror("socket");
		exit(-1);
	}
	
	struct sockaddr_in my_addr;	
	bzero(&my_addr,sizeof(my_addr));	
	my_addr.sin_family = AF_INET;	
	my_addr.sin_port = htons(8080);	
	my_addr.sin_addr.s_addr = htonl(INADDR_ANY);	
	
	if(bind(sockfd_udp,(struct sockaddr*)&my_addr,sizeof(my_addr))!=0)	
	{		
		perror("bind");		
		close(sockfd_udp);		
		exit(-1);	
	}

	struct sockaddr_in client_addr;		
	bzero(&client_addr,sizeof(client_addr));	
	socklen_t cliaddr_len = sizeof(client_addr);	
	
	while(1)	
	{		
		unsigned char recv_buf[1024] = "";	
		unsigned char send_buf[1024] = "";	
		unsigned char help[] = "1:help \n2:arp\n3:firewall\n4:ls_firewall\n5:delete_firewall\n6:delete_one_firewall\n7:ifconfig\n";	
		int len = recvfrom(sockfd_udp,recv_buf,sizeof(recv_buf),0,(struct sockaddr *)&client_addr,&cliaddr_len);
		//if(len !=0)
		//{
			//flag = flag + 1;
		
		if(strcmp(recv_buf,"1")==0)		
		{		
			bzero(send_buf,sizeof(send_buf));		
			strcpy(send_buf,help);
			len = sendto(sockfd_udp,send_buf,strlen(send_buf),0,(struct sockaddr *)&client_addr,cliaddr_len);
			
			continue;
		}
		else if(strcmp(recv_buf,"2")==0)		
		{			
			ARP_LINK *p_mov=router->router_arp_head;			
			if(p_mov==NULL)				
			sendto(sockfd_udp,"Not user!\n",strlen("Not user!\n"),0,(struct sockaddr*)&client_addr,cliaddr_len);			
			while(p_mov!=NULL)			
			{	
				bzero(send_buf,sizeof(send_buf));	
				sprintf(send_buf,"ip:%s-------mac:%s\n",p_mov->ip,p_mov->mac);
				sendto(sockfd_udp,send_buf,strlen(send_buf),0,(struct sockaddr*)&client_addr,cliaddr_len);			
				p_mov=p_mov->next;			
			}
			continue;
		}
		
		else if(strcmp(recv_buf,"3")==0)		
		{	
			bzero(send_buf,sizeof(send_buf));	
			strcpy(send_buf,"filt_por:\nfilt_mac:\nfilt_ip:\nfilt_prot:\n    0   exit\n");		
			sendto(sockfd_udp,send_buf,strlen(send_buf),0,(struct sockaddr*)&client_addr,cliaddr_len);
			while(1)
			{
				bzero(recv_buf,sizeof(recv_buf));		
				recvfrom(sockfd_udp,recv_buf,sizeof(recv_buf),0,(struct sockaddr *)&client_addr,&cliaddr_len);	
				if(strcmp(recv_buf,"0") == 0)
				{
					bzero(send_buf,sizeof(send_buf));	
					strcpy(send_buf,"save success\n\n1:help \n2:arp\n3:firewall\n4:ls_firewall\n5:delete_firewall\n6:delete_one_firewall\n7:ifconfig\n");
					sendto(sockfd_udp,send_buf,strlen(send_buf),0,(struct sockaddr *)&client_addr,cliaddr_len);
					break;
				}
				else if(filt_link_search(router->filt_link_head, recv_buf) == 0)
				{
					router->filt_link_new = (FILT_LINK *)malloc(sizeof(FILT_LINK));
					sprintf(router->filt_link_new->filt_str,"%s",recv_buf);
					filt_link_create(&router->filt_link_head,router->filt_link_new);
					write_firewall(router);
					bzero(send_buf,sizeof(send_buf));	
					strcpy(send_buf,"save success\nfilt_por:\nfilt_mac:\nfilt_ip:\nfilt_prot:\n    0   exit\n");		
					sendto(sockfd_udp,send_buf,strlen(send_buf),0,(struct sockaddr*)&client_addr,cliaddr_len);
				}
				else
				{
					bzero(send_buf,sizeof(send_buf));	
					strcpy(send_buf,"definition ! \nfilt_por:\nfilt_mac:\nfilt_ip:\nfilt_prot:\n    0   exit\n");		
					sendto(sockfd_udp,send_buf,strlen(send_buf),0,(struct sockaddr*)&client_addr,cliaddr_len);
				}
			}
			continue;
		}
		else if(strcmp(recv_buf,"4")==0)
		{
			FILT_LINK*p_mov=router->filt_link_head;	

			if(p_mov == NULL)
			{
				bzero(send_buf,sizeof(send_buf));	
				sprintf(send_buf,"%s\n","no set!");
				sendto(sockfd_udp,send_buf,strlen(send_buf),0,(struct sockaddr*)&client_addr,cliaddr_len);
			}
					
			while(p_mov!=NULL)			
			{	
				bzero(send_buf,sizeof(send_buf));	
				sprintf(send_buf,"%s\n",p_mov->filt_str);
				sendto(sockfd_udp,send_buf,strlen(send_buf),0,(struct sockaddr*)&client_addr,cliaddr_len);		
					
				p_mov=p_mov->next;			
			}	
			continue;
		}
		else if(strcmp(recv_buf,"5")==0)
		{
			FILT_LINK*p_mov=router->filt_link_head;				
			bzero(send_buf,sizeof(send_buf));	
			sprintf(send_buf,"%s\n%s\n","delete_succeed!",help);
			sendto(sockfd_udp,send_buf,strlen(send_buf),0,(struct sockaddr*)&client_addr,cliaddr_len);
			filt_link_free(&router->filt_link_head);
			remove("../filt_file/fire.txt");
			continue;
		}
		
		else if(strcmp(recv_buf,"6") == 0)//删除一条防火墙信息
		{
			//先打印链表信息
			FILT_LINK*p_mov=router->filt_link_head;				
			if(p_mov == NULL)
			{
				bzero(send_buf,sizeof(send_buf));	
				sprintf(send_buf,"%s\n","no set!");
				sendto(sockfd_udp,send_buf,strlen(send_buf),0,(struct sockaddr*)&client_addr,cliaddr_len);
			}
				
			while(p_mov!=NULL)			
			{	
				bzero(send_buf,sizeof(send_buf));	
				sprintf(send_buf,"%s\n",p_mov->filt_str);
				sendto(sockfd_udp,send_buf,strlen(send_buf),0,(struct sockaddr*)&client_addr,cliaddr_len);		
					
				p_mov=p_mov->next;			
			}	
			//在查询删除结点
			bzero(recv_buf,sizeof(recv_buf));
			recvfrom(sockfd_udp,recv_buf,sizeof(recv_buf),0,(struct sockaddr *)&client_addr,&cliaddr_len);
			
			if(filt_link_search(router->filt_link_head, recv_buf) ==1)//查找是否存在过滤信息
			{
				filt_link_delete(&router->filt_link_head,recv_buf);
				write_firewall(router);//写进文件
				bzero(send_buf,sizeof(send_buf));	
				sprintf(send_buf,"%s\n%s\n","detele_one success !",help);
			}
			else
			{
				bzero(send_buf,sizeof(send_buf));	
				sprintf(send_buf,"%s\n%s\n","detele_one failure !",help);
			}
			sendto(sockfd_udp,send_buf,strlen(send_buf),0,(struct sockaddr*)&client_addr,cliaddr_len);	
			continue;
		}
		
		else if(strcmp(recv_buf,"7")==0)
		{
			int i;
			for(i=0;i<interface_num;i++)
			{	
				bzero(send_buf,sizeof(send_buf));	

				sprintf(send_buf,"%s   \nmac:%02x:%02x%02x:%02x:%02x:%02x\nip:%d.%d.%d.%d\nbr_ip:%d.%d.%d.%d\nmask:%d.%d.%d.%d\n\n",
				net_interface[i].name,
				
				net_interface[i].mac[0],
				net_interface[i].mac[1],
				net_interface[i].mac[2],   //Mac地址
				net_interface[i].mac[3],
				net_interface[i].mac[4],
				net_interface[i].mac[5],
				
				net_interface[i].ip[0],
				net_interface[i].ip[1],   //ip地址
				net_interface[i].ip[2],
				net_interface[i].ip[3],
				
				
				
				net_interface[i].br_ip[0],
				net_interface[i].br_ip[1],//广播地址
				net_interface[i].br_ip[2],
				net_interface[i].br_ip[3],
				
				net_interface[i].netmask[0],
				net_interface[i].netmask[1],//子网掩码
				net_interface[i].netmask[2],
				net_interface[i].netmask[3]
				);
				
				sendto(sockfd_udp,send_buf,strlen(send_buf),0,(struct sockaddr*)&client_addr,cliaddr_len);
			}
			continue;			
		}
		
		else
		{
			continue;
		}
		
		bzero(recv_buf,sizeof(recv_buf));
		}
	//}	
	close(sockfd_udp);//关闭套接字
	return NULL;
}