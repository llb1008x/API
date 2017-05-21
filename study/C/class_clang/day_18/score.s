	.file	"score.c"
	.section	.rodata
.LC0:
	.string	"error"
.LC1:
	.string	"E"
.LC2:
	.string	"D"
.LC3:
	.string	"C"
.LC4:
	.string	"B"
.LC5:
	.string	"A"
	.text
.globl fun
	.type	fun, @function
fun:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	movq	%rsp, %rbp
	.cfi_offset 6, -16
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movl	%edi, -20(%rbp)
	movl	$10, %edi
	call	malloc
	movq	%rax, -8(%rbp)
	cmpl	$0, -20(%rbp)
	js	.L2
	cmpl	$100, -20(%rbp)
	jle	.L3
.L2:
	movq	$.LC0, -8(%rbp)
	jmp	.L4
.L3:
	cmpl	$0, -20(%rbp)
	js	.L5
	cmpl	$59, -20(%rbp)
	jg	.L5
	movq	$.LC1, -8(%rbp)
	jmp	.L4
.L5:
	cmpl	$59, -20(%rbp)
	jle	.L6
	cmpl	$69, -20(%rbp)
	jg	.L6
	movq	$.LC2, -8(%rbp)
	jmp	.L4
.L6:
	cmpl	$69, -20(%rbp)
	jle	.L7
	cmpl	$79, -20(%rbp)
	jg	.L7
	movq	$.LC3, -8(%rbp)
	jmp	.L4
.L7:
	cmpl	$79, -20(%rbp)
	jle	.L8
	cmpl	$89, -20(%rbp)
	jg	.L8
	movq	$.LC4, -8(%rbp)
	jmp	.L4
.L8:
	cmpl	$90, -20(%rbp)
	jle	.L4
	movq	$.LC5, -8(%rbp)
.L4:
	movq	-8(%rbp), %rax
	leave
	ret
	.cfi_endproc
.LFE0:
	.size	fun, .-fun
	.ident	"GCC: (Ubuntu/Linaro 4.4.7-1ubuntu2) 4.4.7"
	.section	.note.GNU-stack,"",@progbits
