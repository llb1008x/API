﻿#ifndef __KEY_PTHREAD_H__#define __KEY_PTHREAD_H__#include "../project/types.h"extern void help_information(void);//系统初始界面extern void firewall_information(void);//防火墙设置界面extern void *key_arp_control(void *arg);//远程控制线程#endif//__KEY_PTHREAD_H__