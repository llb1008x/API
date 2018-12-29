	.file	"test.c"
	.section	.rodata
.LC1:
	.string	"a->%d,b->%f\n"
	.text
.globl main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	movq	%rsp, %rbp
	.cfi_offset 6, -16
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movw	$125, -2(%rbp)
	movl	$0x40666666, %eax
	movl	%eax, -8(%rbp)
	cvttss2si	-8(%rbp), %eax
	movw	%ax, -2(%rbp)
	movss	-8(%rbp), %xmm0
	cvttss2si	%xmm0, %eax
	movw	%ax, -2(%rbp)
	movss	-8(%rbp), %xmm0
	cvtps2pd	%xmm0, %xmm0
	movzwl	-2(%rbp), %edx
	movl	$.LC1, %eax
	movl	%edx, %esi
	movq	%rax, %rdi
	movl	$1, %eax
	call	printf
	movl	$0, %eax
	leave
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu/Linaro 4.4.7-1ubuntu2) 4.4.7"
	.section	.note.GNU-stack,"",@progbits
