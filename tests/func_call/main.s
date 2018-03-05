	.file	"main.c"
	.option nopic
	.text
	.align	2
	.globl	foo
	.type	foo, @function
foo:
	addi	sp,sp,-16
	sw	s0,12(sp)
	addi	s0,sp,16
	li	a5,256
	mv	a0,a5
	lw	s0,12(sp)
	addi	sp,sp,16
	jr	ra
	.size	foo, .-foo
	.align	2
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-32
	sw	ra,28(sp)
	sw	s0,24(sp)
	addi	s0,sp,32
	call	foo
	sw	a0,-20(s0)
	lw	a4,-20(s0)
	li	a5,256
	bne	a4,a5,.L4
	li	a5,0
	j	.L6
.L4:
	li	a5,-1
.L6:
	mv	a0,a5
	lw	ra,28(sp)
	lw	s0,24(sp)
	addi	sp,sp,32
	jr	ra
	.size	main, .-main
	.ident	"GCC: (GNU) 7.2.0"
