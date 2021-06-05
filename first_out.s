	.text
.LC0:
	.string	"%d\n"
printint:                           # printf function                               
	pushq	%rbp                    # save the scene
	movq	%rsp, %rbp              # set rsp as the top of stack
	subq	$16, %rsp               
	movl	%edi, -4(%rbp)
	movl	-4(%rbp), %eax          # get the printint() argument
	movl	%eax, %esi
	leaq	.LC0(%rip), %rdi        # get the pointer to "%d\n"
	movl	$0, %eax
	call	printf@PLT              # call printf
	nop
	leave                           # return function
	ret

	.globl	main
	.type	main, @function
main:
	pushq	%rbp
	movq	%rsp, %rbp
	movq	$2, %r8                 # %r8 = 2 
	movq	$3, %r9                 # %r9 = 3
	movq	$5, %r10                # %r10 = 5
	imulq	%r9, %r10               # %r10 = 3*5 = 15
	addq	%r8, %r10               # %r10 = 2 + 15 = 17
	movq	$8, %r8                 # %r8 = 8
	movq	$3, %r9                 # %r9 = 3
	movq	%r8,%rax                # move %r8 to %rax
	cqo                             # Load dividend %rax with 8
	idivq	%r9                     # divided by 3
	movq	%rax,%r8                # save result in %r8
	subq	%r8, %r10               # %r10 = 17 - 2 = 15
	movq	%r10, %rdi              # Copy 15 into %rdi in preparation
	call	printint
	movl	$0, %eax                # Postamble: call exit(0)
	popq	%rbp
	ret
