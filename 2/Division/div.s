	.section	__TEXT,__text,regular,pure_instructions
	.macosx_version_min 10, 13
	.globl	_division               ## -- Begin function division
	.p2align	4, 0x90
_division:                              ## @division
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	%rdx, -24(%rbp)
	movq	%rcx, -32(%rbp)
	movl	$0, -36(%rbp)
LBB0_1:                                 ## =>This Inner Loop Header: Depth=1
	cmpl	$32, -36(%rbp)
	jge	LBB0_8
## %bb.2:                               ##   in Loop: Header=BB0_1 Depth=1
	movl	$2147483648, %eax       ## imm = 0x80000000
	movl	%eax, %ecx
	movq	-32(%rbp), %rdx
	movl	(%rdx), %eax
	shll	$1, %eax
	movl	%eax, (%rdx)
	andq	-8(%rbp), %rcx
	cmpq	$0, %rcx
	je	LBB0_4
## %bb.3:                               ##   in Loop: Header=BB0_1 Depth=1
	movq	-32(%rbp), %rax
	movl	(%rax), %ecx
	addl	$1, %ecx
	movl	%ecx, (%rax)
LBB0_4:                                 ##   in Loop: Header=BB0_1 Depth=1
	movq	-32(%rbp), %rax
	movl	(%rax), %ecx
	movl	%ecx, %eax
	cmpq	-16(%rbp), %rax
	jb	LBB0_6
## %bb.5:                               ##   in Loop: Header=BB0_1 Depth=1
	movl	$1, %eax
	movl	$31, %ecx
	subl	-36(%rbp), %ecx
                                        ## kill: def %cl killed %ecx
	shll	%cl, %eax
	movq	-24(%rbp), %rdx
	orl	(%rdx), %eax
	movl	%eax, (%rdx)
	movq	-16(%rbp), %rdx
	movq	-32(%rbp), %rsi
	movl	(%rsi), %eax
	movl	%eax, %edi
	subq	%rdx, %rdi
	movl	%edi, %eax
	movl	%eax, (%rsi)
LBB0_6:                                 ##   in Loop: Header=BB0_1 Depth=1
	movq	-8(%rbp), %rax
	shlq	$1, %rax
	movq	%rax, -8(%rbp)
## %bb.7:                               ##   in Loop: Header=BB0_1 Depth=1
	movl	-36(%rbp), %eax
	addl	$1, %eax
	movl	%eax, -36(%rbp)
	jmp	LBB0_1
LBB0_8:
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.globl	_main                   ## -- Begin function main
	.p2align	4, 0x90
_main:                                  ## @main
	.cfi_startproc
## %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	subq	$48, %rsp
	movl	$0, -4(%rbp)
	movl	%edi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-16(%rbp), %rsi
	movq	8(%rsi), %rdi
	callq	_atoll
	movq	%rax, -24(%rbp)
	movq	-16(%rbp), %rax
	movq	16(%rax), %rdi
	callq	_atoll
	leaq	-36(%rbp), %rdx
	leaq	-40(%rbp), %rcx
	movq	%rax, -32(%rbp)
	movl	$0, -36(%rbp)
	movl	$0, -40(%rbp)
	movq	-24(%rbp), %rdi
	movq	-32(%rbp), %rsi
	callq	_division
	leaq	L_.str(%rip), %rdi
	movq	-24(%rbp), %rsi
	movq	-32(%rbp), %rdx
	movl	-36(%rbp), %ecx
	movl	-40(%rbp), %r8d
	movb	$0, %al
	callq	_printf
	xorl	%ecx, %ecx
	movl	%eax, -44(%rbp)         ## 4-byte Spill
	movl	%ecx, %eax
	addq	$48, %rsp
	popq	%rbp
	retq
	.cfi_endproc
                                        ## -- End function
	.section	__TEXT,__cstring,cstring_literals
L_.str:                                 ## @.str
	.asciz	"%lu / %lu = %d R %d\n"


.subsections_via_symbols
