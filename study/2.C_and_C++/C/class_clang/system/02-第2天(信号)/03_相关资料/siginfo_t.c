/*信号设置结构体：siginfo_t*/
typedef struct siginfo
{
	int si_signo;		/* Signal number.  */
	int si_errno;		/* If non-zero, an errno value associated with
				   this signal, as defined in <errno.h>.  */
	int si_code;		/* Signal code.  */

	union
	{
		int _pad[__SI_PAD_SIZE];

		 /* kill().  */
		struct
		{
			__pid_t si_pid;	/* Sending process ID.  */
			__uid_t si_uid;	/* Real user ID of sending process.  */
		} _kill;

		/* POSIX.1b timers.  */
		struct
		{
			int si_tid;		/* Timer ID.  */
			int si_overrun;	/* Overrun count.  */
			sigval_t si_sigval;	/* Signal value.  */
		} _timer;

		/* POSIX.1b signals.  */
		struct
		{
			__pid_t si_pid;	/* Sending process ID.  */
			__uid_t si_uid;	/* Real user ID of sending process.  */
			sigval_t si_sigval;	/* Signal value.  */
		} _rt;

		/* SIGCHLD.  */
		struct
		{
			__pid_t si_pid;	/* Which child.  */
			__uid_t si_uid;	/* Real user ID of sending process.  */
			int si_status;	/* Exit value or signal.  */
			__clock_t si_utime;
			__clock_t si_stime;
		} _sigchld;

		/* SIGILL, SIGFPE, SIGSEGV, SIGBUS.  */
		struct
		{
			void *si_addr;	/* Faulting insn/memory ref.  */
		} _sigfault;

		/* SIGPOLL.  */
		struct
		{
			long int si_band;	/* Band event for SIGPOLL.  */
			int si_fd;
		} _sigpoll;
	} _sifields;
} siginfo_t;
  
