#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "lrc.h"
#include "./pos/console.h"
extern TIME m_time;
extern char * song_msg[4];//保存前四行歌曲信息首地址
/*************************************************************************
*	函数功能:读出文件内容
*	参数： 
* 		src_file_name:文件名字，从此文件中读取内容。 
*	返回值:读出字符串的首地址
**************************************************************************/
int max_lrc_pointer_num;
char * read_src_file(char *src_file_name)
{
	unsigned long int FILE_SIZE=0;
	char *src_file_text=NULL;
	FILE  *fp=NULL;
	
	fp=fopen(src_file_name,"rb");
	if(fp==NULL)
	{
		printf("open %s faild\n",src_file_name);
		return 0;
	}
	
	fseek(fp,0,SEEK_END);
	FILE_SIZE=ftell(fp);
	
	rewind(fp);					//一定要把“文件指针”移动到文件开始处
	src_file_text=(char *)malloc(FILE_SIZE+1);
	memset(src_file_text,'\0',FILE_SIZE+1);
	if(fread(src_file_text,FILE_SIZE,1,fp)==0)
	{
		printf("read from src_file faild\n");
		return NULL;
	}
	fclose(fp);					// 关闭文件
	return src_file_text ;
}
/************************************************************************
*	函数功能:切割 歌词数组
*	参数：
*		lrc_text 指针数组，让其指针指向切出来的内容 
* 		file_buf 歌词内容数组
* 	返回值:切割出来的字符串的个数
*************************************************************************/
int strtok_lrc_buf(char *lrc_text[],char *file_buf)
{
	int i=0;
	
	lrc_text[i]=strtok(file_buf,"\r\n");
	while(lrc_text[i]!=NULL)
	{
		lrc_text[++i]=strtok(NULL,"\r\n");
	}
	return i;
}


/**************************************************************************
*	函数功能:解析lrc前四行，歌曲的信息如歌手、专辑等。。
*	参数：
*		lrc_text 指针数组 lrc 中的前四行 
*		song_msg 指针数组 用来指向处理好的歌曲信息
* 	返回值:无
**************************************************************************/
void lrc_head_resolve(char *song_msg[],char * lrc_text[])
{
	int i=0;
	char * message[]={"[ti","[ar","[al","[by"};
	char * temp;
	for(i=0;i<4;i++)// 循环四次 找到四个头信息的开始
	{

		if (strncmp(lrc_text[i],message[i],2)==0)
		{
			temp = lrc_text[i]; 
			/*
				[ti:兰亭序]
				[ar:李维]
				[al:情已逝 DSD]
				[by:朱古力]
			*/
			temp=strrchr(temp,']');//找到每一行的最后一个 ']'
			if(temp!=NULL)
				*temp='\0';
			 song_msg[i]=lrc_text[i]+4;				//跳过 ti: 三个字母,保存歌词信息;
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
void print_song_message(char * song_msg[])
{
	int i=0;
	char *name[4]={"歌名:","歌手:","专辑:","制作:"};
	clear_screen();
	cusor_hide();
	set_fg_color(COLOR_BLUE);
	for(i=0;i<4;i++)
	{
		cusor_moveto(35,i+1);
		printf("%s  %s\n",name[i],song_msg[i]);
	}
}


/**************************************************************************
*	函数功能:解析歌曲信息
*	参数：
*		lrc_text 指向歌词内容(以行的方式存储)的指针数组
*		lrc_line 歌词的行数
* 	返回值:无
**************************************************************************/
LRC* lrc_resolve(int lrc_line,char * lrc_text[])
{
	LRC *head=NULL,*pi;
	int i=0,j=0;
	char *lrc_str;
	int time_buf[10];		//用来暂时记录歌词中的时间,单位为：毫秒
	int time_num=0;				//用来暂时记录本句歌词有多少个时间与其对应
	char buf1[3],buf2[3];
	for(i=4;i<lrc_line;i++)
	{
		lrc_str=lrc_text[i];//一行的歌词：[02:04.94][00:36.09]我想大声宣布 对你依依不舍
		time_num=0;		//解析每句歌词时，要把本句歌词对应的时间数初始化为0
		while(*lrc_str == '[')//解析一行的所有时间
		{
			strncpy(buf1,lrc_str+1,2);
			buf1[2]='\0';
			strncpy(buf2,lrc_str+4,2);
			buf2[2]='\0';
			time_buf[time_num++]=atoi(buf1)*60+atoi(buf2);
			lrc_str=lrc_str+10;
		}//解析完一行的所有时间后 lrc_str即为歌词的首地址
		//根据本句歌词对应的时间个数，把歌词与时间 保存到链表中
		for(j=0;j<time_num;j++)
		{
			pi =  ( LRC *)malloc(sizeof(LRC)); //申请节点，存放解析出歌词与时间
			pi->time = time_buf[j];
			strcpy (pi->lrc_buf,lrc_str);	
			
			//将解析出的时间、歌词内容插入到单向链表并进行排序	
			head = InsertList(head,pi); 
		}
	}
	pi=head;
	i=0;
	while(pi!=NULL)//使链表中每个节点有个节点号，即第几句歌词
	{
		pi->lrc_cur_num=i++;
		max_lrc_pointer_num=pi->lrc_cur_num;
		pi=pi->next;
	}
	return head;
}

LRC* link_search(LRC *head,int m_time)
{
	LRC *pb=head;
	while(pb!=NULL)
	{
		if(pb->time==m_time)
			return pb;
		else
			pb=pb->next;
	}
	return NULL;
}
/**************************************************************************
*	函数功能:往链表中插入歌词节点。
*	参数：
*		head 链表的头节点地址。   
*		lrc_link 将要插入的节点首地址。
* 	返回值:插入之后链表的头节点的地址
**************************************************************************/
LRC *InsertList(LRC* head,LRC *lrc_link)
{
	LRC *pb,*pf;
	if(head==NULL)// 如果原先链表为空 head为pi
	{
		lrc_link->next=NULL;
		head=lrc_link;
		return head;
	}
	pb=pf=head;
	while( (lrc_link->time > pb->time)  && (pb->next!=NULL))// 查找比pi->num  更大的节点 
	{	
		pf=pb;
		pb=pb->next;
	}
	if(lrc_link->time <= pb->time)	// 如果上面的while循环式因为找到 lrc_link->time <= pb->time 而推出的
	{
		if(pb==head)				// 如果找到的节点为第一个，则pi应该放到head的前面
		{
			lrc_link->next=head;	// 把pi放到链表里  
			head=lrc_link;			// 把pi变成头节点
		}
		else 						// 如果是pb普通节点的话  在pf和pb之间插入pi
		{
			pf->next=lrc_link;		// pb的前一个节点的指针域 指向pi
			lrc_link->next=pb;		// pi的指针域指向pb  
		}
	}
	else							// 如果链表中的节点的time都比 pi->time 小，则插到链表尾端
	{
		pb->next=lrc_link; 			// pb的指针域指向 pi 
		lrc_link->next=NULL;		// pi 的下一个为NULL 即pi为链表的尾
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
void link_free(LRC *head)
{
	LRC *pb;
	while(head!=NULL)
	{
		pb=head;
		head=head->next;
		free(pb);
	}
}
/************************************************************************
*	函数功能:延时函数
*	参数：
*		sec ：延时的秒数
* 	返回值:无
*************************************************************************/
void time_delay(int sec)
{
	int s_time,e_time;
	s_time=time(NULL);
	while(1)
	{
		e_time=time(NULL);
		if(e_time==s_time+sec)
			break;
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
	//cusor_moveto(0,6);
}

/************************************************************************
*	函数功能:获取 开始显示的歌词的节点num
*	参数：
*		find_num: 找到的节点的num
* 	返回值:开始显示的节点的num
*************************************************************************/
int get_show_num(int find_num)
{
	int show_num=0;
	if(find_num < 2)//前两句
	{
		show_num=0;
	}
	else if(find_num>max_lrc_pointer_num-4)//最后五句
		show_num=max_lrc_pointer_num-4;
	else
		show_num=find_num -2;//show_num为从第几个节点开始显示
	return show_num;
}

/************************************************************************
*	函数功能:打印链表中5个节点的信息到屏幕上
*	参数：
*		pb:打印的第一个节点
		expect_play_pointer: 要高亮的节点
* 	返回值:无
*************************************************************************/
void lrc_printf(LRC *pb , LRC *expect_play_pointer)
{
	int i;
	for(i=0;i<5;i++)//从pb开始打印5句歌词 
	{
		if(pb==expect_play_pointer)//高亮红色显示节点
			color_pos_printf(pb->lrc_buf,30,6+i,COLOR_RED);
		else//普通蓝色显示
			color_pos_printf(pb->lrc_buf,30,6+i,COLOR_BLUE);
		pb=pb->next;
	}
}
/************************************************************************
*	函数功能:在屏幕上打印歌词
*	参数：
*		head：歌词链表的头
		expect_play_pointer：要高亮显示的歌词的节点
* 	返回值:切割出来的字符串的个数
*************************************************************************/
void lrc_show(LRC *head,LRC *expect_play_pointer)
{
	int i=0;
	LRC * pb=NULL;
	int show_num;
	pb=head;
	show_num=get_show_num(expect_play_pointer->lrc_cur_num);
	//将pb指针挪动到开始打印的节点
	for(i=0;i<show_num;i++)// 让pb指向开始显示的节点
	{
		pb=pb->next;
	}
	print_song_message(song_msg);//打印歌曲信息
	time_printf(m_time);//打印时间
	lrc_printf(pb,expect_play_pointer);//从pb开始打印五个节点的信息,expect_player_pointer 要高亮显示的节点
}














