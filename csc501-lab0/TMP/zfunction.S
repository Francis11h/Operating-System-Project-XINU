	.text
	.globl zfunction
zfunction:
	pushl %ebp
	movl %esp,%ebp
	andl $0xFF803FFF, 8(%ebp)
	sall $4, 8(%ebp)
	andl $0xFFFFFFF0, 8(%ebp)
	movl 8(%ebp), %eax
	leave
	ret