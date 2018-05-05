/*头文件路径：include/linux/shm.h */
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

/*对于每个IPC对象，
系统共用一个struct ipc_perm的数据结构来存放权限信息，
以确定一个ipc操作是否可以访问该IPC对象。*/ 
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