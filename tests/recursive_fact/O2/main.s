	.file	"main.c"
	.option nopic
	.section	.text.startup,"ax",@progbits
	.align	2
	.globl	main
	.type	main, @function
main:
	li	a0,0
	ret
	.size	main, .-main
	.text
	.align	2
	.globl	factorial
	.type	factorial, @function
factorial:
	mv	a5,a0
	li	a4,1
	li	a0,1
	bleu	a5,a4,.L6
.L5:
	mul	a0,a0,a5
	addi	a5,a5,-1
	bne	a5,a4,.L5
	ret
.L6:
	ret
	.size	factorial, .-factorial
	.ident	"GCC: (GNU) 7.2.0"
