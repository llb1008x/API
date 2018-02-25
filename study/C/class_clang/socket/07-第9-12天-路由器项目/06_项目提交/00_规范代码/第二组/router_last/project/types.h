#ifndef TYPES_H_
#define TYPES_H_

//shell文字修饰
#define PRINT(X,Y) {	write(1,Y,5);  \
						printf(X);   \
						fflush(stdout); \
						write(1,"\e[0m",4); \
					}
#define GREEN 	"\e[32m"   //shell打印显示绿色
#define RED     "\e[31m"   //shell打印显示红色
#define YELLOW     "\e[33m"   //shell打印显示黄色
#define BLUE     "\e[34m"   //shell打印显示蓝色
#define PURPLE     "\e[35m"   //shell打印显示紫色
#define SKY     "\e[36m"   //shell打印显示天蓝色
#define WHITE     "\e[37m"   //shell打印显示白色
#define BLACK     "\e[38m"   //shell打印显示红色

extern int flag;



typedef struct _route
{
	unsigned char ip[16]; //源ip
	unsigned char mac[18];//源Mac
	struct _route *next;
}ARP_LINK;

typedef struct _filt
{
	unsigned char filt_str[100];//防火墙内容
	struct _filt *next;
}FILT_LINK;

typedef struct router
{
	int sockfd;//原始套接字
	int fd;//文件标示符
	
	ARP_LINK *router_arp_head;//路由器链表头
	ARP_LINK *router_arp_new;//路由器新节点
	
	FILT_LINK *filt_link_head;//防火墙链表头
	FILT_LINK *filt_link_new;//防火墙新节点
}ROU;
 /*********************************************************************
* 任务回调函数，也可根据需要自行修改
*********************************************************************/
typedef void *(*pool_task_f)(void *arg);

/*********************************************************************
* 任务句柄
*********************************************************************/
typedef struct _task{
	pool_task_f process;/*回调函数，任务运行时会调用此函数，注意也可声明成其它形式*/
	void *arg;     /*回调函数的参数*/
	struct _task *next;
}pool_task;
/*********************************************************************
* 线程池句柄
*********************************************************************/
typedef struct
{
	pthread_t *threadid;		/* 线程号 */
	int threads_limit;			/* 线程池中允许的活动线程数目 */
	int destroy_flag;			/* 是否销毁线程池 , 0销毁，1不销毁*/
	pool_task *queue_head;	/* 链表结构，线程池中所有等待任务 */
	int task_in_queue;			/* 当前等待队列的任务数目 */
	pthread_mutex_t queue_lock;	/* 锁 */
	pthread_cond_t queue_ready;	/* 条件变量 */
}pool_t;


#endif