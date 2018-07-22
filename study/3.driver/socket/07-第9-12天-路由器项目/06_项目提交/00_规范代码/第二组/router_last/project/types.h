#ifndef TYPES_H_
#define TYPES_H_

//shell��������
#define PRINT(X,Y) {	write(1,Y,5);  \
						printf(X);   \
						fflush(stdout); \
						write(1,"\e[0m",4); \
					}
#define GREEN 	"\e[32m"   //shell��ӡ��ʾ��ɫ
#define RED     "\e[31m"   //shell��ӡ��ʾ��ɫ
#define YELLOW     "\e[33m"   //shell��ӡ��ʾ��ɫ
#define BLUE     "\e[34m"   //shell��ӡ��ʾ��ɫ
#define PURPLE     "\e[35m"   //shell��ӡ��ʾ��ɫ
#define SKY     "\e[36m"   //shell��ӡ��ʾ����ɫ
#define WHITE     "\e[37m"   //shell��ӡ��ʾ��ɫ
#define BLACK     "\e[38m"   //shell��ӡ��ʾ��ɫ

extern int flag;



typedef struct _route
{
	unsigned char ip[16]; //Դip
	unsigned char mac[18];//ԴMac
	struct _route *next;
}ARP_LINK;

typedef struct _filt
{
	unsigned char filt_str[100];//����ǽ����
	struct _filt *next;
}FILT_LINK;

typedef struct router
{
	int sockfd;//ԭʼ�׽���
	int fd;//�ļ���ʾ��
	
	ARP_LINK *router_arp_head;//·��������ͷ
	ARP_LINK *router_arp_new;//·�����½ڵ�
	
	FILT_LINK *filt_link_head;//����ǽ����ͷ
	FILT_LINK *filt_link_new;//����ǽ�½ڵ�
}ROU;
 /*********************************************************************
* ����ص�������Ҳ�ɸ�����Ҫ�����޸�
*********************************************************************/
typedef void *(*pool_task_f)(void *arg);

/*********************************************************************
* ������
*********************************************************************/
typedef struct _task{
	pool_task_f process;/*�ص���������������ʱ����ô˺�����ע��Ҳ��������������ʽ*/
	void *arg;     /*�ص������Ĳ���*/
	struct _task *next;
}pool_task;
/*********************************************************************
* �̳߳ؾ��
*********************************************************************/
typedef struct
{
	pthread_t *threadid;		/* �̺߳� */
	int threads_limit;			/* �̳߳�������Ļ�߳���Ŀ */
	int destroy_flag;			/* �Ƿ������̳߳� , 0���٣�1������*/
	pool_task *queue_head;	/* ����ṹ���̳߳������еȴ����� */
	int task_in_queue;			/* ��ǰ�ȴ����е�������Ŀ */
	pthread_mutex_t queue_lock;	/* �� */
	pthread_cond_t queue_ready;	/* �������� */
}pool_t;


#endif