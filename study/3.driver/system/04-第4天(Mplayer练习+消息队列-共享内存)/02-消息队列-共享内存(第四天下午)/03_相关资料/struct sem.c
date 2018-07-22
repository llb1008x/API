
/* One semaphore structure for each semaphore in the system. */
struct sem
{
	int	semval;		/* current value *�ź�����ֵ*/
	int	sempid;		/* pid of last operation *���һ�������ź����Ľ��̺�*/
	struct list_head sem_pending; /* pending single-sop operations */
};

