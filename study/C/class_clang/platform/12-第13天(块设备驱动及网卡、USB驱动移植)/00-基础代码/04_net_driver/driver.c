/*
* Copyright (c) 2016 sunplusedu
* This is a demo, about Virtual network card driver
*
* Date and Dition:	2016-06-14	v1.0
* Author:				<haotian.su@sunplusapp.com>
* 
*/

/*
* debug:
* 		ifconfig eth0 down
* 		ifconfig eth1 up
* 		ifconfig eth1 172.20.223.111
*/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <plat/gpio-cfg.h>
#include <mach/gpio.h>
#include <linux/ioport.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/netdevice.h>
#include <linux/crc32.h>
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/ethtool.h>
#include <linux/sched.h>
#include <linux/timer.h>
#include <linux/bug.h>
#include <linux/bitops.h>
#include <linux/io.h>
#include <linux/swab.h>
#include <linux/phy.h>
#include <linux/device.h>
#include <linux/etherdevice.h> /* eth_type_trans */
#include <linux/ip.h>
#include <linux/if_arp.h>

#define LOGIC_DEBUG 1

static struct net_device *n_dev;

static int driver_net_open(struct net_device *dev)
{
	printk("%s,%d\n",__FUNCTION__,__LINE__);

	return 0;
}

static int driver_net_stop(struct net_device *dev)
{
	printk("%s,%d\n",__FUNCTION__,__LINE__);

	return 0;
}

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

static void set_sk_buff(struct sk_buff *rx_skb,struct sk_buff *skb,int off)
{
	unsigned char pseudo_head[1024];

	*(rx_skb->data+off+0) = n_dev->dev_addr[0];
	*(rx_skb->data+off+1) = n_dev->dev_addr[1];
	*(rx_skb->data+off+2) = n_dev->dev_addr[2];
	*(rx_skb->data+off+3) = n_dev->dev_addr[3];
	*(rx_skb->data+off+4) = n_dev->dev_addr[4];
	*(rx_skb->data+off+5) = n_dev->dev_addr[5];
	
	*(rx_skb->data+off+6) = 0x2c;
	*(rx_skb->data+off+7) = 0x60;
	*(rx_skb->data+off+8) = 0x0c;
	*(rx_skb->data+off+9) = 0xb8;
	*(rx_skb->data+off+10) = 0xbd;
	*(rx_skb->data+off+11) = 0x6b;

	*(rx_skb->data+off+12) = 0x08;
	*(rx_skb->data+off+13) = 0x00;
	*(rx_skb->data+off+14) = 0x45;
	*(rx_skb->data+off+15) = 0x00;
	*(rx_skb->data+off+16) = 0x00;
	*(rx_skb->data+off+17) = 0x30;
	*(rx_skb->data+off+18) = 0x60;
	*(rx_skb->data+off+19) = 0x52;
	*(rx_skb->data+off+20) = 0x00;
	*(rx_skb->data+off+21) = 0x00;
	*(rx_skb->data+off+22) = 0x80;
	*(rx_skb->data+off+23) = 0x11;

	*(rx_skb->data+off+24) = 0x00;
	*(rx_skb->data+off+25) = 0x00;

	*(rx_skb->data+off+26) = *(skb->data+off+30);
	*(rx_skb->data+off+27) = *(skb->data+off+31);
	*(rx_skb->data+off+28) = *(skb->data+off+32);
	*(rx_skb->data+off+29) = *(skb->data+off+33);
	
	*(rx_skb->data+off+30) = *(skb->data+off+26);
	*(rx_skb->data+off+31) = *(skb->data+off+27);
	*(rx_skb->data+off+32) = *(skb->data+off+28);
	*(rx_skb->data+off+33) = *(skb->data+off+29);

	*(rx_skb->data+off+34) = *(skb->data+off+34);
	*(rx_skb->data+off+35) = *(skb->data+off+35);
	*(rx_skb->data+off+36) = *(skb->data+off+36);
	*(rx_skb->data+off+37) = *(skb->data+off+37);

	memcpy(rx_skb->data+42, (skb->data)+42, ((skb->len)-42));

	*((unsigned short *)(rx_skb->data+16)) = htons(20+8+(skb->len)-42);//IP总长度 = 20 + 8 + len
	*((unsigned short *)(rx_skb->data+14+20+4)) = htons(8+(skb->len)-42);//udp总长度 = 8 + len

	pseudo_head[0] = *(skb->data+off+30);
	pseudo_head[1] = *(skb->data+off+31);
	pseudo_head[2] = *(skb->data+off+32);
	pseudo_head[3] = *(skb->data+off+33);
	pseudo_head[4] = *(skb->data+off+26);
	pseudo_head[5] = *(skb->data+off+27);
	pseudo_head[6] = *(skb->data+off+28);
	pseudo_head[7] = *(skb->data+off+29);
	pseudo_head[8] = 0x00;
	pseudo_head[9] = 17;
	pseudo_head[10] = 0x00;
	pseudo_head[11] = 0x00;
	*((unsigned short *)&pseudo_head[10]) = htons(8 + (skb->len)-42);//为头部中的udp长度（和真实udp长度是同一个值）
	memcpy(pseudo_head+12, (rx_skb->data)+34, ((skb->len)-42+8));//--计算udp校验和时需要加上伪头部--
	//5.对IP首部进行校验
	pseudo_head[18] = 0;
	pseudo_head[19] = 0;
	*((unsigned short *)(rx_skb->data+24)) = htons(checksum((unsigned short *)((rx_skb->data)+14),20/2));
	//6.--对UDP数据进行校验--
	*((unsigned short *)(rx_skb->data+40)) = htons(checksum((unsigned short *)pseudo_head,(12+8+(skb->len)-42)/2));
}

static int driver_start_xmit (struct sk_buff *skb,struct net_device *dev)
{
	struct sk_buff *rx_skb;
	
	/* 构造一个sk_buff */
	rx_skb = dev_alloc_skb(skb->len);
	/* 拷贝数据包 */
	rx_skb->data = rx_skb->head;
	skb_reset_tail_pointer(rx_skb);
	/* Align IP on 16B boundary */
	skb_reserve(rx_skb, NET_IP_ALIGN);
	skb_put(rx_skb, skb->len);
	/* 准备sk_buff */
	set_sk_buff(rx_skb,skb,0);
	/* 提交sk_buff */	
	rx_skb->protocol = eth_type_trans(rx_skb, dev);
	rx_skb->ip_summed = CHECKSUM_NONE;
	netif_receive_skb(rx_skb);
	/* 更新统计 */
	dev->stats.rx_packets++;
	dev->stats.rx_bytes += skb->len;

	dev->stats.tx_packets++;
	dev->stats.tx_bytes += skb->len;

	return NETDEV_TX_OK;
}

static int net_set_mac_address(struct net_device *dev,void *p)
{
	struct sockaddr *addr = p;
	int i;

	printk("%s,%d\nMAC",__FUNCTION__,__LINE__);
	if (!is_valid_ether_addr(addr->sa_data))
		return -EADDRNOTAVAIL;

	memcpy(dev->dev_addr, addr->sa_data, ETH_ALEN);
	for(i=0;i<6;i++)
		printk(":%x",dev->dev_addr[i]);
	printk("\n");
	//xxx_set_hw_mac_address(dev->dev_addr);
		
	return 0;
}

static const struct net_device_ops nd_ops = {
	.ndo_open = driver_net_open,
	.ndo_stop = driver_net_stop,
	.ndo_start_xmit = driver_start_xmit,
	.ndo_set_mac_address = net_set_mac_address,
};

static const struct ethtool_ops et_ops = {
	
};

static int __init driver_net_init(void)
{	
	printk("%s,%d\n",__FUNCTION__,__LINE__);

	n_dev = alloc_etherdev(sizeof(int));
	n_dev->ethtool_ops = &et_ops;
	n_dev->netdev_ops = &nd_ops;

	ether_setup(n_dev);	
	n_dev->flags |= IFF_MULTICAST;		
	n_dev->dev_addr[0] = 0x00;	
	n_dev->dev_addr[1] = 0x53;	
	n_dev->dev_addr[2] = 0x50;	
	n_dev->dev_addr[3] = 0x00;	
	n_dev->dev_addr[4] = 0x57;	
	n_dev->dev_addr[5] = 0xe1;	
	n_dev->flags |= IFF_NOARP;	
	
	n_dev->features = NETIF_F_NO_CSUM|NETIF_F_SG | NETIF_F_FRAGLIST
		| NETIF_F_TSO
		| NETIF_F_HIGHDMA
		| NETIF_F_LLTX
		| NETIF_F_NETNS_LOCAL;/* netdevice.h */
	register_netdev(n_dev);

	return 0;
}

static void __exit driver_net_exit(void)
{
	printk("%s,%d\n",__FUNCTION__,__LINE__);
	unregister_netdev(n_dev);
	free_netdev(n_dev);
}

module_init(driver_net_init);
module_exit(driver_net_exit);

MODULE_AUTHOR("sunplusedu");
MODULE_LICENSE("GPL");
