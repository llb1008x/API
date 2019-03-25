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
//语法格式： 	LRC_INFO *insert_list( LRC_INFO * head, LRC_INFO* pi)
//实现功能：	将解析出的歌词按顺序插入链表
//参数：		head: 待排序链表
//				pi:待插入节点
//返回值：		排好序的链表首地址
//=============================================================
LRC_INFO *insert_list( LRC_INFO * head, LRC_INFO* pi)
{
	 LRC_INFO *pf,*pb;
	 pb=head;
	
	 if(head==NULL) 					//原表为空表，则直接加入
	 {
		 head=pi;
		 pi->next=NULL;
	 }
	 else							//原表非空
	 {
		 while( (pi->time > pb->time) && (pb->next!=NULL) )	//从头开始找到比插入时间表小的节点
		 {
			 pf=pb;						//pf指向当前节点的前一个节点，节点后移
			 pb=pb->next;
		 }							
		 if(pi->time <= pb->time)		//找到所要插入节点位置
		 {
			 if(head==pb)//如果第2个节点时间小于头节点，则与头节点交换位置
			 {
				 head=pi;			//在第一结点之前插入
				 pi->next = pb;	//add by xiuhai
			 }
			 else 
			 {	
				 pf->next=pi;		//在中间位置插入
				 pi->next=pb;
			 }	
		 }
		 else
		 {
			 pb->next=pi;
			 pi->next=NULL;
		 } 							//在表末插入
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
	LRC_INFO *pb;
	while(head!=NULL)
	{
		pb=head;
		head=head->next;
		free(pb);
	}
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
	LRC_INFO *p = head;

	while(p!=NULL)
	{
		printf("%d,%s\n",p->time,p->lrc_buf);
		p = p->next;
	}
}

LRC_INFO *play_song(unsigned int time,LRC_INFO *head,char song_msg[HEAD_NUM][HEAD_SIZE])
{
	int i = 0;
	static int count=0;//count记录显示到了那一句歌词
	static int flag=0;
	static LRC_INFO *temp;

	if(flag==0)
	{
		temp=head;
		flag=1;
	}
	if(count<6)
	{
		if( time == ( temp->time / 100 ) )	//与时间标签对比，判断是否该显示歌词
		{			
			system("clear");	//清屏						
			print_song_message(song_msg);//打印歌曲信息			
			for(i=0;i<count;i++)	//显示当前句的前6句
			{
				color_pos_printf(head->lrc_buf,30,6+i,COLOR_BLUE);
				head = head->next;			
			}
			color_pos_printf(temp->lrc_buf,30,6+count,COLOR_RED);
			temp = temp->next;				//显示相应歌词，调整歌词链表到下一个节点
			count++;
		}	
	}
	else
	{
	
		if( time == ( temp->time / 100 ) )	
		{
			system("clear");	//清屏			
			print_song_message(song_msg);//打印歌曲信息			
			for(i=0;i<count-6;i++) //count<=6时，head指向头不变，当count>6时，
			{					   	//将head移到当前节点的前6个节点处		
				head = head->next;				
			}
			for(i=0;i<6;i++)	//显示当前句的前6句
			{
				color_pos_printf(head->lrc_buf,30,6+i,COLOR_BLUE);
				head = head->next;			
			}
			color_pos_printf(temp->lrc_buf,30,6+i,COLOR_RED);
			temp = temp->next;				
			count++;
		}
	}
	return temp;
}	











