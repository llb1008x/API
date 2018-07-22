/*信号设置结构体：ucontext_t
The <ucontext.h> header defines the mcontext_t type through typedef.
The <ucontext.h> header defines the ucontext_t type as a structure 
that includes at least the following members: */
typedef struct ucontext{
	/*pointer to the context that will be resumed when this context returns*/
	struct ucontext *uc_link;	
	/*the stack used by this context*/
	stack_t uc_stack;	
	/*a machine-specific representation of the saved context*/
	mcontext_t uc_mcontext;	
	/*the set of signals that are blocked when this context is active*/
	__sigset_t uc_sigmask;
}ucontext_t;

