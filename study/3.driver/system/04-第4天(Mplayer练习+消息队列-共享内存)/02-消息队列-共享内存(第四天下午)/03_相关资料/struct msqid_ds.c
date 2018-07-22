
/* Obsolete, used only for backwards compatibility and libc5 compiles */
struct semid_ds {
	struct ipc_perm	sem_perm;		/* permissions .. see ipc.h *权限*/
	__kernel_time_t	sem_otime;		/* last semop time *最近semop的时间*/
	__kernel_time_t	sem_ctime;		/* last change time *最近修改的时间*/
	struct sem	*sem_base;		/* ptr to first semaphore in array *信号量集中第一个信号量*/
	struct sem_queue *sem_pending;		/* pending operations to be processed *阻塞信号量*/
	struct sem_queue **sem_pending_last;	/* last pending operation *最后一个阻塞信号量*/
	struct sem_undo	*undo;			/* undo requests on this array *undo队列*/
	unsigned short	sem_nsems;		/* no. of semaphores in array *信号量集中信号量的个数*/
};