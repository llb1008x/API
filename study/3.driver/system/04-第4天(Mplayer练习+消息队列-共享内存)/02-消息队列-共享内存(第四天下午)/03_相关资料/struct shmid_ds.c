/*ͷ�ļ�·����include/linux/shm.h */
struct shmid_ds 
{ 
	struct ipc_perm shm_perm; /* operation perms */ 
	int shm_segsz; /* size of segment (bytes) */ 
	__kernel_time_t shm_atime; /* last attach time */ 
	__kernel_time_t shm_dtime; /* last detach time */ 
	__kernel_time_t shm_ctime; /* last change time */ 
	__kernel_ipc_pid_t shm_cpid; /* pid of creator */ 
	__kernel_ipc_pid_t shm_lpid; /* pid of last operator */ 
	unsigned short shm_nattch; /* no. of current attaches */ 
	unsigned short shm_unused; /* compatibility */ 
	void *shm_unused2; /* ditto - used by DIPC */ 
	void *shm_unused3; /* unused */ 
};

/*����ÿ��IPC����
ϵͳ����һ��struct ipc_perm�����ݽṹ�����Ȩ����Ϣ��
��ȷ��һ��ipc�����Ƿ���Է��ʸ�IPC����*/ 
struct ipc_perm 
{ 
	__kernel_key_t key; 
	__kernel_uid_t uid;
	__kernel_gid_t gid; 
	__kernel_uid_t cuid; 
	__kernel_gid_t cgid;
	__kernel_mode_t mode; 
	unsigned short seq; 
};