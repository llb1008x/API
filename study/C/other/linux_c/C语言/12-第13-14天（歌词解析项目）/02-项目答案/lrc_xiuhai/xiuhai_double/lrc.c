#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "lrc.h"
#include "./pos/console.h"

/*************************************************************************
*	函数功能:读出文件内容
*	参数： 
* 		src_file_name:文件名字，从此文件中读取内容。 
*	返回值:读出字符串的首地址
**************************************************************************/
int max_lrc_pointer_num;
char * read_src_file(char *src_file_name,unsigned int *file_len)
{
	char *src_file_text=NULL;
	FILE  *fp=NULL;
	
	fp=fopen(src_file_name,"rb");
	if(fp==NULL)
	{
		printf("open %s faild\n",src_file_name);
		return NULL;
	}
	
	fseek(fp,0,SEEK_END);
	*file_len=ftell(fp);
	
	rewind(fp);					//一定要把“文件指针”移动到文件开始处
	src_file_text=(char *)malloc(*file_len+1);
	memset(src_file_text,'\0',*file_len+1);
	if(fread(src_file_text,*file_len,1,fp)==0)
	{
		printf("read from src_file faild\n");
		return NULL;
	}
	fclose(fp);					// 关闭文件
	return src_file_text ;
}

/**************************************************************************
*	函数功能:解析lrc前四行，歌曲的信息如歌手、专辑等。。
*	参数：
*		lrc_text 指针数组 lrc 中的前四行 
*		song_msg 指针数组 用来指向处理好的歌曲信息
* 	返回值:无
**************************************************************************/
void lrc_head_resolve(char *lrc_buffer,char song_msg[HEAD_NUM][HEAD_SIZE])
{
	int offset=0;
	int temp,i=0;

	char *message[HEAD_NUM]={"[ti","[ar","[al","[by"}; //"歌名:","歌手:","专辑:","制作:"

	for(i=0;i<HEAD_NUM;i++)
	{
		while( lrc_buffer[offset] != '[' )
			offset++;
	
		if (strncmp(lrc_buffer+offset,message[i],3)==0)
		{
			temp = offset+4;  					//跳过 ti: 三个字母
			while( lrc_buffer[offset] !=']' )
				offset++;     					//找到歌曲名的结尾
			lrc_buffer[offset]='\0';
			strcpy(song_msg[i],&lrc_buffer[temp]);
		}
	}
}

/**************************************************************************
*	函数功能:打印歌曲信息
*	参数：
*		song_msg 歌曲信息 指针数组指向歌曲头信息内容
*	返回值:无
*	注意点：无
**************************************************************************/
void print_song_message(char song_msg[HEAD_NUM][HEAD_SIZE])
{
	int i=0;
	char *name[HEAD_NUM]={"歌名:","歌手:","专辑:","制作:"};
	clear_screen();
	cusor_hide();
	set_fg_color(COLOR_BLUE);
	for(i=0;i<HEAD_NUM;i++)
	{
		cusor_moveto(35,i+1);
		printf("%s  %s\n",name[i],song_msg[i]);
	}
}


/**************************************************************************
*	函数功能:解析歌曲信息
*	参数：
*		lrc_file 指向歌词内容
*		file_size 歌词的大小
* 	返回值:创建的链表首地址
**************************************************************************/
LRC_INFO* lrc_resolve(char *lrc_file,unsigned int file_size)
{
	LRC_INFO *head= NULL,*lrc_link;	//歌词节点
	signed int offset_time = 0;	//时间补偿值
	unsigned int cur_offset = 0;
	signed int cur_time[10] = {0}; //保存同一句歌词重复出现的时间点，最多重复10次
	int num=0,i=0; //num表示同一句歌词重复出现的次数
	int sign = 0;//表示时间补偿的符号正负

	while(cur_offset < file_size)						//循环解析出所有的歌词
	{
		if(lrc_file[cur_offset] == '[')		
		{
			cur_offset++;
			
			if(strncmp(&lrc_file[cur_offset], "offset:", 7) == 0)	//检测到时间补偿标签
			{
				cur_offset += 7;
				if(lrc_file[cur_offset] == '-')
				{
					sign = -1;
					cur_offset++;
				}
				else
					sign = 1;
				while(lrc_file[cur_offset] != ']')//组合正整数
				{
					offset_time *= 10;
					offset_time += lrc_file[cur_offset++] - '0';
				}
				offset_time = offset_time / 10 * sign;	//将时间补偿值毫秒变成10ms为单位
			}
			
			if((lrc_file[cur_offset]) >= '0' && (lrc_file[cur_offset]) <= '9'  ) //检测到歌词时间标签
			{
				char *lrc;

				while(lrc_file[cur_offset] >='0' && lrc_file[cur_offset] <='9')//取出这句歌词所有时间标签
				{
					cur_time[num++] = ( (  (lrc_file[cur_offset] - '0') * 10 + (lrc_file[cur_offset + 1] - '0')) * 60 + \
						((lrc_file[cur_offset + 3] - '0') * 10 + lrc_file[cur_offset + 4] - '0')) * 100 + (lrc_file[cur_offset + 6] - '0') * 10 + lrc_file[cur_offset + 7] - '0';

					cur_offset=cur_offset+10;		//解析一句歌词在不同时间出现的情况
				}

				lrc = &lrc_file[cur_offset-1];//得到这句歌词首地址
				while( lrc_file[cur_offset] !='\n')
					cur_offset++;
				lrc_file[cur_offset] =  '\0' ;		//修改buffer将'\n'换成'\0'，构成多个字符串
				
				for(i=0;i<num;i++)//跟据这句歌词得复出现时间插入到链表各节点中
				{
					lrc_link =  ( LRC_INFO *)malloc(sizeof(LRC_INFO));  
					lrc_link->time = cur_time[i]+offset_time;
					strcpy (lrc_link->lrc_buf,lrc);			//申请节点，存放解析出歌词
					//将解析出的时间、歌词内容插入到单向链表并进行排序	
					head = insert_list(head,lrc_link); 
				}
				num=0;				
			}
		}
		cur_offset++;
	} 
	return head;
}

//=============================================================
// 语法格式：	insert_list(LRC_INFO * head,LRC_INFO * pi)
// 实现功能：	将新申请的节点加入到指定链表中，并按照time进行从小到大排序
// 参数：		*head:待插入链表
//				* pi：带插入节点
// 返回值：		插入指定节点后的新链表首址
//=============================================================
LRC_INFO * insert_list(LRC_INFO * head, LRC_INFO * pi)
{
	LRC_INFO *pb, *pf;
	pf = pb = head;
	
	if(head == NULL)//如果链表为空，则建立，注意空间在传入前申请好
	{
		head = pi;
		pi->next = head;
		pi->prior = head;
	}
	else
	{
		while((pi->time > pb->time) && (pb->next != head))//找到一个比插入值大的节点，然后插在它前面
		{
			pf = pb;//pf指向前， pb指向后
			pb = pb->next;
		}
		
		if(pi->time <= pb->time)//找到要插入的节点位置，插到pb的前面
		{
			if(head == pb)//在第一节点之前插入
			{
				pi->prior = head->prior;//让新头部的前指针指向尾节点
				head->prior->next = pi;	//让尾部的后指针指向新头部节点
				pi->next = pb;			//让新头部的后指针指向旧头部节点
				pb->prior = pi;			//让旧头部的前指针指向新头部节点
				head = pi;				//让头部指针指向新头部节点	
			}
			else//在中间插入
			{
				pf->next = pi;
				pi->prior = pf;
				pi->next = pb;
				pb->prior = pi;
			}
		}
		else//只有pb->head为空才会成立
		{
			pb->next = pi;//在表末插入
			pi->prior = pb;
			pi->next = head;
			head->prior = pi;//头始终指向新插入的节点
		}
	}
	
	return head;
}

/**************************************************************************
*	函数功能:回收链表
*	参数：
*		head 链表的头节点地址。   
*		
* 	返回值:无
**************************************************************************/
void link_free(LRC_INFO *head)
{
	LRC_INFO *pf,*pb = head;
	
	while(pb->next!=pb)
	{
		pf=pb;
		pb=pb->next;
		pb->prior = pf->prior;
		pf->prior->next = pb;	
		free(pf);
	}
	if(pb != NULL)
		free(pb);	
}

/************************************************************************
*	函数功能:在屏幕的x,y位置，用color颜色打印一个字符串str
*	参数：
*		str：要打印的字符串的首地址
		x:显示在屏幕上的x坐标
		y:显示在屏幕上的y坐标
		color: 显示字符串的颜色
* 	返回值:无
*************************************************************************/
void color_pos_printf(char *str,int x,int y,int color)
{
			cusor_moveto(x,y);
			set_fg_color(color);
			printf("%s\n",str);
}
/************************************************************************
*	函数功能:在屏幕上打印时间
*	参数：
*		m_time: 时间结构体
* 	返回值:无
*************************************************************************/
void  time_printf(TIME m_time)
{
	char str[30];
	sprintf(str,"%02d:%02d",m_time.minute,m_time.second);
	color_pos_printf(str,40,5,COLOR_BLUE);
}

//=============================================================
//语法格式： 	void print_link( LRC_INFO * head)
//实现功能：	打印指定链表各节点所有内容
//参数：		head: 待排序链表首地址
//返回值：		无
//=============================================================
void print_link( LRC_INFO * head)
{
	LRC_INFO *pb = head;

	if (pb == NULL)
		return;
	while(pb->next != head)
	 {
		printf("%d,%s\n",pb->time,pb->lrc_buf);
		pb=pb->next;
	 }
	printf("%d,%s\n",pb->time,pb->lrc_buf);
}

void lrc_printf(LRC_INFO *pb , LRC_INFO *head,int num)
{
	int i;
	
	color_pos_printf(pb->lrc_buf,30,6,COLOR_RED);
	pb=pb->next;
	for(i=0;((i<num)&&(pb != head));i++)
	{
		color_pos_printf(pb->lrc_buf,30,6+i+1,COLOR_BLUE);
		pb=pb->next;
	}
}

LRC_INFO *play_song(unsigned int time,LRC_INFO *head,char song_msg[HEAD_NUM][HEAD_SIZE])
{
	static int count=0;//count记录显示到了那一句歌词
	static LRC_INFO *temp = NULL;

	if(count == 0){
		temp=head;
		count++;
	}
	else
	{
		if( time == ( temp->time / 100 ) )	//与时间标签对比，判断是否该显示歌词
		{			
			system("clear");	//清屏						
			print_song_message(song_msg);//打印歌曲信息		
			lrc_printf(temp,head,SPACE_SIZE);
			temp = temp->next;				//显示相应歌词，调整歌词链表到下一个节点
			count++;
			return temp;
		}
	}
	return NULL;
}	











