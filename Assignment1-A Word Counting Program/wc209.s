	.file	"wc209.c"
	.globl	SPACE
	.section	.rodata
	.align 4
	.type	SPACE, @object
	.size	SPACE, 4
SPACE:
	.zero	4
	.globl	ENTER
	.align 4
	.type	ENTER, @object
	.size	ENTER, 4
ENTER:
	.long	1
	.globl	SLASH
	.align 4
	.type	SLASH, @object
	.size	SLASH, 4
SLASH:
	.long	2
	.globl	STAR
	.align 4
	.type	STAR, @object
	.size	STAR, 4
STAR:
	.long	3
	.globl	CHAR
	.align 4
	.type	CHAR, @object
	.size	CHAR, 4
CHAR:
	.long	4
	.globl	N_EOF
	.align 4
	.type	N_EOF, @object
	.size	N_EOF, 4
N_EOF:
	.long	5
	.comm	ch,4,4
	.globl	comment_start_l
	.data
	.align 4
	.type	comment_start_l, @object
	.size	comment_start_l, 4
comment_start_l:
	.long	-1
	.comm	l,4,4
	.comm	w,4,4
	.globl	c
	.bss
	.align 4
	.type	c, @object
	.size	c, 4
c:
	.zero	4
	.globl	state
	.align 4
	.type	state, @object
	.size	state, 4
state:
	.zero	4
	.comm	arrow,4,4
	.globl	word_comment
	.align 4
	.type	word_comment, @object
	.size	word_comment, 4
word_comment:
	.zero	4
	.text
	.globl	determine_arrow
	.type	determine_arrow, @function
determine_arrow:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	call	__ctype_b_loc@PLT
	movq	(%rax), %rax
	movl	-4(%rbp), %edx
	movslq	%edx, %rdx
	addq	%rdx, %rdx
	addq	%rdx, %rax
	movzwl	(%rax), %eax
	movzwl	%ax, %eax
	andl	$8192, %eax
	testl	%eax, %eax
	je	.L2
	cmpl	$10, -4(%rbp)
	jne	.L3
	movl	$1, %eax
	jmp	.L4
.L3:
	movl	$0, %eax
	jmp	.L4
.L2:
	cmpl	$47, -4(%rbp)
	jne	.L5
	movl	$2, %eax
	jmp	.L4
.L5:
	cmpl	$42, -4(%rbp)
	jne	.L6
	movl	$3, %eax
	jmp	.L4
.L6:
	cmpl	$-1, -4(%rbp)
	jne	.L7
	movl	$5, %eax
	jmp	.L4
.L7:
	movl	$4, %eax
.L4:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	determine_arrow, .-determine_arrow
	.section	.rodata
.LC0:
	.string	"Unknown arrow occur!! : %d"
.LC1:
	.string	"Unknown ARROW occur!! : %d"
.LC2:
	.string	"Unknown ARROW occur!! : %d\n"
.LC3:
	.string	"Unknown STATE occur!! : %d\n"
	.text
	.globl	update_state
	.type	update_state, @function
update_state:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	cmpl	$5, -8(%rbp)
	ja	.L9
	movl	-8(%rbp), %eax
	leaq	0(,%rax,4), %rdx
	leaq	.L11(%rip), %rax
	movl	(%rdx,%rax), %eax
	movslq	%eax, %rdx
	leaq	.L11(%rip), %rax
	addq	%rdx, %rax
	jmp	*%rax
	.section	.rodata
	.align 4
	.align 4
.L11:
	.long	.L10-.L11
	.long	.L12-.L11
	.long	.L13-.L11
	.long	.L14-.L11
	.long	.L15-.L11
	.long	.L16-.L11
	.text
.L10:
	movl	c(%rip), %eax
	addl	$1, %eax
	movl	%eax, c(%rip)
	movl	$0, %eax
	cmpl	%eax, -4(%rbp)
	jne	.L17
	movl	$1, -8(%rbp)
	jmp	.L24
.L17:
	movl	$1, %eax
	cmpl	%eax, -4(%rbp)
	jne	.L19
	movl	$1, -8(%rbp)
	movl	l(%rip), %eax
	addl	$1, %eax
	movl	%eax, l(%rip)
	jmp	.L24
.L19:
	movl	$2, %eax
	cmpl	%eax, -4(%rbp)
	jne	.L20
	movl	$2, -8(%rbp)
	jmp	.L24
.L20:
	movl	$3, %eax
	cmpl	%eax, -4(%rbp)
	je	.L21
	movl	$4, %eax
	cmpl	%eax, -4(%rbp)
	jne	.L22
.L21:
	movl	$5, -8(%rbp)
	jmp	.L24
.L22:
	movl	$5, %eax
	cmpl	%eax, -4(%rbp)
	jne	.L23
	movl	$6, -8(%rbp)
	movl	c(%rip), %eax
	subl	$1, %eax
	movl	%eax, c(%rip)
	jmp	.L24
.L23:
	movq	stderr(%rip), %rax
	movl	-4(%rbp), %edx
	leaq	.LC0(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	jmp	.L24
.L12:
	movl	c(%rip), %eax
	addl	$1, %eax
	movl	%eax, c(%rip)
	movl	$0, %eax
	cmpl	%eax, -4(%rbp)
	jne	.L25
	movl	$1, -8(%rbp)
	jmp	.L24
.L25:
	movl	$1, %eax
	cmpl	%eax, -4(%rbp)
	jne	.L27
	movl	$1, -8(%rbp)
	movl	l(%rip), %eax
	addl	$1, %eax
	movl	%eax, l(%rip)
	jmp	.L24
.L27:
	movl	$2, %eax
	cmpl	%eax, -4(%rbp)
	jne	.L28
	movl	$2, -8(%rbp)
	jmp	.L24
.L28:
	movl	$3, %eax
	cmpl	%eax, -4(%rbp)
	je	.L29
	movl	$4, %eax
	cmpl	%eax, -4(%rbp)
	jne	.L30
.L29:
	movl	$5, -8(%rbp)
	jmp	.L24
.L30:
	movl	$5, %eax
	cmpl	%eax, -4(%rbp)
	jne	.L31
	movl	$6, -8(%rbp)
	movl	l(%rip), %eax
	addl	$1, %eax
	movl	%eax, l(%rip)
	movl	c(%rip), %eax
	subl	$1, %eax
	movl	%eax, c(%rip)
	jmp	.L24
.L31:
	movq	stderr(%rip), %rax
	movl	-4(%rbp), %edx
	leaq	.LC1(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	jmp	.L24
.L13:
	movl	c(%rip), %eax
	addl	$1, %eax
	movl	%eax, c(%rip)
	movl	$0, %eax
	cmpl	%eax, -4(%rbp)
	jne	.L32
	movl	$1, -8(%rbp)
	movl	w(%rip), %eax
	addl	$1, %eax
	movl	%eax, w(%rip)
	movl	$0, word_comment(%rip)
	jmp	.L24
.L32:
	movl	$1, %eax
	cmpl	%eax, -4(%rbp)
	jne	.L34
	movl	$1, -8(%rbp)
	movl	l(%rip), %eax
	addl	$1, %eax
	movl	%eax, l(%rip)
	movl	w(%rip), %eax
	addl	$1, %eax
	movl	%eax, w(%rip)
	movl	$0, word_comment(%rip)
	jmp	.L24
.L34:
	movl	$2, %eax
	cmpl	%eax, -4(%rbp)
	jne	.L35
	movl	$2, -8(%rbp)
	movl	$1, word_comment(%rip)
	jmp	.L24
.L35:
	movl	$3, %eax
	cmpl	%eax, -4(%rbp)
	jne	.L36
	movl	$3, -8(%rbp)
	movl	l(%rip), %eax
	addl	$1, %eax
	movl	%eax, comment_start_l(%rip)
	movl	c(%rip), %eax
	subl	$1, %eax
	movl	%eax, c(%rip)
	movl	word_comment(%rip), %eax
	testl	%eax, %eax
	je	.L37
	movl	w(%rip), %eax
	addl	$1, %eax
	movl	%eax, w(%rip)
.L37:
	movl	$0, word_comment(%rip)
	jmp	.L24
.L36:
	movl	$4, %eax
	cmpl	%eax, -4(%rbp)
	jne	.L38
	movl	$5, -8(%rbp)
	movl	$0, word_comment(%rip)
	jmp	.L24
.L38:
	movl	$5, %eax
	cmpl	%eax, -4(%rbp)
	jne	.L39
	movl	$6, -8(%rbp)
	movl	l(%rip), %eax
	addl	$1, %eax
	movl	%eax, l(%rip)
	movl	c(%rip), %eax
	subl	$1, %eax
	movl	%eax, c(%rip)
	movl	w(%rip), %eax
	addl	$1, %eax
	movl	%eax, w(%rip)
	movl	$0, word_comment(%rip)
	jmp	.L24
.L39:
	movq	stderr(%rip), %rax
	movl	-4(%rbp), %edx
	leaq	.LC1(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	jmp	.L24
.L14:
	movl	$0, %eax
	cmpl	%eax, -4(%rbp)
	je	.L40
	movl	$2, %eax
	cmpl	%eax, -4(%rbp)
	je	.L40
	movl	$4, %eax
	cmpl	%eax, -4(%rbp)
	jne	.L41
.L40:
	movl	$3, -8(%rbp)
	jmp	.L24
.L41:
	movl	$1, %eax
	cmpl	%eax, -4(%rbp)
	jne	.L43
	movl	c(%rip), %eax
	addl	$1, %eax
	movl	%eax, c(%rip)
	movl	l(%rip), %eax
	addl	$1, %eax
	movl	%eax, l(%rip)
	movl	$3, -8(%rbp)
	jmp	.L24
.L43:
	movl	$3, %eax
	cmpl	%eax, -4(%rbp)
	jne	.L44
	movl	$4, -8(%rbp)
	jmp	.L24
.L44:
	movl	$5, %eax
	cmpl	%eax, -4(%rbp)
	jne	.L45
	movl	$7, -8(%rbp)
	movl	l(%rip), %eax
	addl	$1, %eax
	movl	%eax, l(%rip)
	jmp	.L24
.L45:
	movq	stderr(%rip), %rax
	movl	-4(%rbp), %edx
	leaq	.LC1(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	jmp	.L24
.L15:
	movl	$0, %eax
	cmpl	%eax, -4(%rbp)
	je	.L46
	movl	$4, %eax
	cmpl	%eax, -4(%rbp)
	jne	.L47
.L46:
	movl	$3, -8(%rbp)
	jmp	.L24
.L47:
	movl	$1, %eax
	cmpl	%eax, -4(%rbp)
	jne	.L49
	movl	c(%rip), %eax
	addl	$1, %eax
	movl	%eax, c(%rip)
	movl	l(%rip), %eax
	addl	$1, %eax
	movl	%eax, l(%rip)
	movl	$3, -8(%rbp)
	jmp	.L24
.L49:
	movl	$3, %eax
	cmpl	%eax, -4(%rbp)
	jne	.L50
	movl	$4, -8(%rbp)
	jmp	.L24
.L50:
	movl	$2, %eax
	cmpl	%eax, -4(%rbp)
	jne	.L51
	movl	$1, -8(%rbp)
	jmp	.L24
.L51:
	movl	$5, %eax
	cmpl	%eax, -4(%rbp)
	jne	.L52
	movl	$7, -8(%rbp)
	movl	l(%rip), %eax
	addl	$1, %eax
	movl	%eax, l(%rip)
	jmp	.L24
.L52:
	movq	stderr(%rip), %rax
	movl	-4(%rbp), %edx
	leaq	.LC1(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	jmp	.L24
.L16:
	movl	c(%rip), %eax
	addl	$1, %eax
	movl	%eax, c(%rip)
	movl	$0, %eax
	cmpl	%eax, -4(%rbp)
	jne	.L53
	movl	$1, -8(%rbp)
	movl	w(%rip), %eax
	addl	$1, %eax
	movl	%eax, w(%rip)
	jmp	.L24
.L53:
	movl	$1, %eax
	cmpl	%eax, -4(%rbp)
	jne	.L55
	movl	$1, -8(%rbp)
	movl	w(%rip), %eax
	addl	$1, %eax
	movl	%eax, w(%rip)
	movl	l(%rip), %eax
	addl	$1, %eax
	movl	%eax, l(%rip)
	jmp	.L24
.L55:
	movl	$2, %eax
	cmpl	%eax, -4(%rbp)
	jne	.L56
	movl	$2, -8(%rbp)
	movl	$1, word_comment(%rip)
	jmp	.L24
.L56:
	movl	$3, %eax
	cmpl	%eax, -4(%rbp)
	je	.L57
	movl	$4, %eax
	cmpl	%eax, -4(%rbp)
	jne	.L58
.L57:
	movl	$5, -8(%rbp)
	jmp	.L24
.L58:
	movl	$5, %eax
	cmpl	%eax, -4(%rbp)
	jne	.L59
	movl	$6, -8(%rbp)
	movl	w(%rip), %eax
	addl	$1, %eax
	movl	%eax, w(%rip)
	movl	l(%rip), %eax
	addl	$1, %eax
	movl	%eax, l(%rip)
	movl	c(%rip), %eax
	subl	$1, %eax
	movl	%eax, c(%rip)
	jmp	.L24
.L59:
	movq	stderr(%rip), %rax
	movl	-4(%rbp), %edx
	leaq	.LC2(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	jmp	.L24
.L9:
	movq	stderr(%rip), %rax
	movl	-8(%rbp), %edx
	leaq	.LC3(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	nop
.L24:
	movl	-8(%rbp), %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	update_state, .-update_state
	.section	.rodata
.LC4:
	.string	"%d %d %d\n"
	.align 8
.LC5:
	.string	"Error: line %d: unterminated comment\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
.L63:
	call	getchar@PLT
	movl	%eax, ch(%rip)
	movl	ch(%rip), %eax
	movl	%eax, %edi
	call	determine_arrow
	movl	%eax, arrow(%rip)
	movl	state(%rip), %edx
	movl	arrow(%rip), %eax
	movl	%edx, %esi
	movl	%eax, %edi
	call	update_state
	movl	%eax, state(%rip)
	movl	state(%rip), %eax
	cmpl	$6, %eax
	je	.L62
	movl	state(%rip), %eax
	cmpl	$7, %eax
	jne	.L63
.L62:
	movl	state(%rip), %eax
	cmpl	$6, %eax
	jne	.L64
	movl	c(%rip), %ecx
	movl	w(%rip), %edx
	movl	l(%rip), %eax
	movl	%eax, %esi
	leaq	.LC4(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, %eax
	jmp	.L65
.L64:
	movl	comment_start_l(%rip), %edx
	movq	stderr(%rip), %rax
	leaq	.LC5(%rip), %rsi
	movq	%rax, %rdi
	movl	$0, %eax
	call	fprintf@PLT
	movl	$1, %eax
.L65:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 5.5.0-1ubuntu2) 5.4.1 20171010"
	.section	.note.GNU-stack,"",@progbits
