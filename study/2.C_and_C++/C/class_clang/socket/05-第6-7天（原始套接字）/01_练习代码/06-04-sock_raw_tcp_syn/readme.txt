改程序运行注意事项：
1.在虚拟机终端添加如下代码：（让其不发送RST）
	iptables -A OUTPUT -p tcp --tcp-flags ALL RST
	iptables -A OUTPUT -p tcp --tcp-flags ALL RST -j DROP

2.在组SYN包时TCP中的req(序列号)不能为0，可以写成1，ack_req 为0

3.在组ACK包时：
	a.必须从MAC->IP->TCP重新组包，不能只修改TCP报文头的信息
	b.tcp报文头中的req写成2，ack_req必须为对方发过来的req+1(不能直接写1哟)

4.在组发送第一次带普通数据的TCP数据包时：（要先运行一个服务器）
	a.必须从MAC->IP->TCP重新组包，不能只修改TCP报文头的信息
	b.tcp报文头中的req写成2，ack_req必须为对方发过来的req+1(不能直接写1哟)

5.在组发送第二次带普通数据的TCP数据包时：（如果你愿意）
	a.必须从MAC->IP->TCP重新组包，不能只修改TCP报文头的信息
	b.tcp报文头中的req写成2+2(间隔为2)，ack_req必须为对方发过来的req+1(不能直接写1哟)