	.file	"main.c"
	.option nopic
	.text
	.align	2
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-32
	sw	ra,28(sp)
	sw	s0,24(sp)
	addi	s0,sp,32
	li	a0,9
	call	factorial
	sw	a0,-20(s0)
	lw	a4,-20(s0)
	li	a5,364544
	addi	a5,a5,-1664
	bne	a4,a5,.L2
	li	a5,0
	j	.L4
.L2:
	li	a5,-1
.L4:
	mv	a0,a5
	lw	ra,28(sp)
	lw	s0,24(sp)
	addi	sp,sp,32
	jr	ra
	.size	main, .-main
	.align	2
	.globl	factorial
	.type	factorial, @function
factorial:
	addi	sp,sp,-32
	sw	ra,28(sp)
	sw	s0,24(sp)
	addi	s0,sp,32
	sw	a0,-20(s0)
	lw	a4,-20(s0)
	li	a5,1
	bgtu	a4,a5,.L6
	li	a5,1
	j	.L7
.L6:
	lw	a5,-20(s0)
	addi	a5,a5,-1
	mv	a0,a5
	call	factorial
	mv	a4,a0
	lw	a5,-20(s0)
	mul	a5,a4,a5
.L7:
	mv	a0,a5
	lw	ra,28(sp)
	lw	s0,24(sp)
	addi	sp,sp,32
	jr	ra
	.size	factorial, .-factorial
	.ident	"GCC: (GNU) 7.2.0"
