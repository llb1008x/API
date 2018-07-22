#ifndef _TYPES_H_
#define _TYPES_H_

#include<gtk/gtk.h>
#include<glade/glade.h>
#include<pthread.h>
#include<semaphore.h>
#include"./sungtk_interface/sungtk_interface.h"
#include"./LRC/lyrics_process.h"

#define A8_SCREEN_WIDTH 800
#define A8_SCREEN_HEIGHT 480


#define PATH_SIZE 128						/*·����С*/
#define SONG_SIZE 128						/*������С*/


#define SONG_PATH "../song/"
#define LRC_PATH "../lyrics/"

typedef struct
{
	GladeXML *gxml;
	GtkWidget * win_back;			/*������,������*/
	GtkWidget * win_back_fixed;		/*���ڹ̶�����*/
	GtkWidget * win_back_tabel;		/*���ڱ�񲼾�*/
	GtkWidget * win_list_eventbox;	/*�����б��ź���*/
	GtkWidget * win_list_fixed;		/*�����б���*/

	SunGtkCList * win_list;			/*�����б�*/

	GtkWidget * volume_event_box;
	GtkWidget * volume_fixed;
	GtkWidget * mute;	/*������ť*/
	GtkWidget * volume_rate;
	GtkWidget * volume_rate_fixed;

	GtkWidget * lrc_light;	/*�����ʾ*/


	GtkWidget * rate;	/*������*/
	GtkWidget * pause;	/*��ͣ��ť*/	
	GtkWidget * next;	//��һ����ť
	GtkWidget * back;	//��һ����ť
	
	GtkWidget * song;	/*��ʾ��ǰ���Ÿ���*/
	GtkWidget * singer;	//�質��
	GtkWidget * time;	/*��ʾ��ǰ����ʱ��*/
	GtkWidget * length;	/*�����ܳ���*/
	GtkWidget * image_artist;	/*������*/
}PLAYER_WIN;

/******����״̬��Ϣ*****/
typedef struct
{
	char song_cur[SONG_SIZE];		//��ǰ���ŵĸ���
	int ispause;               	 	//mplayer��ͣ��־λ��0��δ��ͣ��1������ͣ
	int ismute;						//������־λ
	int play_change_flag;			//�л�������־λ
	int volume;                 	//mplayer������Χ��0-100��
	int temp_volume;
	int play_model;					//����ģʽ
	int song_cur_num;				//��ǰ���Ÿ������
	int percent;					//���Ž��Ȱٷֱ�
	int time_position;				//���Ž���ʱ��
	int song_length;				//����ʱ���ܳ���

	pthread_rwlock_t status_rwlock; //״̬��д��
}SONG_STATUS;

/******�����б�*******/
typedef struct 
{
	char **song_list;				//�����б�ָ��
	int song_num;               	//��������

	LRC_PTR lrc;

	pthread_rwlock_t list_rwlock;	//�б��д��
}SONG_LIST;

/*********·�����*********/
typedef struct
{
	char song_path[PATH_SIZE];				//����·��
	char fifo_path[PATH_SIZE];				//��������ܵ�·����
	char lrc_path[PATH_SIZE];				//��Ÿ��·����
	char pic_path[PATH_SIZE];				//���ͼƬ·����

	pthread_rwlock_t path_rwlock;			//·����д��
}PATH;

/*******mplayer��������*******/
typedef struct
{
	pid_t pid;						//mplayer�������̺�
	int play_flag;					//mplayer������־λ;0:δ����,1:������

	pthread_rwlock_t process_rwlock;
}PLAYER_PROCESS;


/*******�򿪵������ܵ��ļ�������********/
typedef struct
{
	int cmd_fifo_fd;				//��mplayer����������ļ�������
	int msg_fifo_fd;				//����mplayer��Ϣ���ļ�������
	pthread_mutex_t write_cmd_mutex;//д����������ܵ�������
	
	sem_t sem_wr;					//����������ź���
	sem_t sem_rd;					//����Mplayer��Ϣ���ź���

	pthread_mutex_t rate_cmd_mutex;
}FIFO_FD;


/*******����mplayer�����ṹ��*********/
typedef struct 
{
	PLAYER_WIN win;					//Mplayer����
	SONG_STATUS status;				//����״̬
	SONG_LIST list;					//�����б�
	PATH path;						//����·��
	PLAYER_PROCESS process;
	FIFO_FD fifo;					//�����ܵ�
}PLAYER;



#endif
