
/* Obsolete, used only for backwards compatibility and libc5 compiles */
struct semid_ds {
	struct ipc_perm	sem_perm;		/* permissions .. see ipc.h *Ȩ��*/
	__kernel_time_t	sem_otime;		/* last semop time *���semop��ʱ��*/
	__kernel_time_t	sem_ctime;		/* last change time *����޸ĵ�ʱ��*/
	struct sem	*sem_base;		/* ptr to first semaphore in array *�ź������е�һ���ź���*/
	struct sem_queue *sem_pending;		/* pending operations to be processed *�����ź���*/
	struct sem_queue **sem_pending_last;	/* last pending operation *���һ�������ź���*/
	struct sem_undo	*undo;			/* undo requests on this array *undo����*/
	unsigned short	sem_nsems;		/* no. of semaphores in array *�ź��������ź����ĸ���*/
};