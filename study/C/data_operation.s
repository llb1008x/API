	.file	"data_operation.c"
	.section	.rodata
.LC1:
	.string	"%d,%lf\n"
	.text
.globl main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	andl	$-16, %esp
	subl	$48, %esp
	movl	$6, 44(%esp)
	movl	$0x40666666, %eax
	movl	%eax, 40(%esp)
	flds	40(%esp)
	fnstcw	30(%esp)
	movzwl	30(%esp), %eax
	movb	$12, %ah
	movw	%ax, 28(%esp)
	fldcw	28(%esp)
	fistpl	44(%esp)
	fldcw	30(%esp)
	flds	40(%esp)
	fldcw	28(%esp)
	fistpl	44(%esp)
	fldcw	30(%esp)
	flds	40(%esp)
	movl	$.LC1, %eax
	fstpl	8(%esp)
	movl	44(%esp), %edx
	movl	%edx, 4(%esp)
	movl	%eax, (%esp)
	call	printf
	movl	$0, %eax
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (Ubuntu/Linaro 4.4.7-8ubuntu1) 4.4.7"
	.section	.note.GNU-stack,"",@progbits
