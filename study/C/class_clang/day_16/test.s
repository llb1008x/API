	.file	"test.c"
	.section	.rodata
.LC0:
	.string	"RAD1->%f\n"
.LC2:
	.string	"RAD2->%f\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$.LC0, %eax
	movsd	.LC1(%rip), %xmm0
	movq	%rax, %rdi
	movl	$1, %eax
	call	printf
	movl	$.LC2, %eax
	movsd	.LC3(%rip), %xmm0
	movq	%rax, %rdi
	movl	$1, %eax
	call	printf
	movl	$0, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.section	.rodata
	.align 8
.LC1:
	.long	3229815407
	.long	1074340298
	.align 8
.LC3:
	.long	3229815407
	.long	1075388874
	.ident	"GCC: (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3"
	.section	.note.GNU-stack,"",@progbits
