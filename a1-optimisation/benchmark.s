	.file	"benchmark.c"
# GNU C17 (GCC) version 12.2.1 20220819 (Red Hat 12.2.1-1) (x86_64-redhat-linux)
#	compiled by GNU C version 12.2.1 20220819 (Red Hat 12.2.1-1), GMP version 6.2.1, MPFR version 4.1.0-p13, MPC version 1.2.1, isl version none
# GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
# options passed: -mtune=generic -march=x86-64
	.text
	.section	.rodata
.LC0:
	.string	"The sum from 1 to %i is %i,\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp	#
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp	#,
	.cfi_def_cfa_register 6
	subq	$32, %rsp	#,
# benchmark.c:10: long int size = 1000000000;
	movq	$1000000000, -24(%rbp)	#, size
# benchmark.c:12: long int note = 0;
	movq	$0, -8(%rbp)	#, note
# benchmark.c:13: for (long int ix = 0; ix < size; ++ix)
	movq	$0, -16(%rbp)	#, ix
# benchmark.c:13: for (long int ix = 0; ix < size; ++ix)
	jmp	.L2	#
.L3:
# benchmark.c:15: note += ix;
	movq	-16(%rbp), %rax	# ix, tmp84
	addq	%rax, -8(%rbp)	# tmp84, note
# benchmark.c:13: for (long int ix = 0; ix < size; ++ix)
	addq	$1, -16(%rbp)	#, ix
.L2:
# benchmark.c:13: for (long int ix = 0; ix < size; ++ix)
	movq	-16(%rbp), %rax	# ix, tmp85
	cmpq	-24(%rbp), %rax	# size, tmp85
	jl	.L3	#,
# benchmark.c:17: printf("The sum from 1 to %i is %i,\n", size, note);
	movq	-8(%rbp), %rdx	# note, tmp86
	movq	-24(%rbp), %rax	# size, tmp87
	movq	%rax, %rsi	# tmp87,
	movl	$.LC0, %edi	#,
	movl	$0, %eax	#,
	call	printf	#
	movl	$0, %eax	#, _8
# benchmark.c:21: }
	leave	
	.cfi_def_cfa 7, 8
	ret	
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (GNU) 12.2.1 20220819 (Red Hat 12.2.1-1)"
	.section	.note.GNU-stack,"",@progbits
