
/* One semaphore structure for each semaphore in the system. */
struct sem
{
	int	semval;		/* current value *信号量的值*/
	int	sempid;		/* pid of last operation *最后一个操作信号量的进程号*/
	struct list_head sem_pending; /* pending single-sop operations */
};

