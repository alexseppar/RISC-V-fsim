	.file	"main.c"
	.option nopic
	.text
	.align	2
	.globl	swap
	.type	swap, @function
swap:
	lw	a4,0(a1)
	lw	a5,0(a0)
	sw	a4,0(a0)
	sw	a5,0(a1)
	ret
	.size	swap, .-swap
	.align	2
	.globl	bubbleSort
	.type	bubbleSort, @function
bubbleSort:
	addi	a5,a1,-1
	blez	a5,.L3
	slli	a2,a1,2
	add	a2,a0,a2
	mv	a1,a5
	addi	a0,a0,4
.L5:
	mv	a5,a0
.L7:
	lw	a4,-4(a5)
	lw	a3,0(a5)
	ble	a4,a3,.L6
	sw	a3,-4(a5)
	sw	a4,0(a5)
.L6:
	addi	a5,a5,4
	bne	a2,a5,.L7
	addi	a1,a1,-1
	addi	a2,a2,-4
	bnez	a1,.L5
.L3:
	ret
	.size	bubbleSort, .-bubbleSort
	.section	.text.startup,"ax",@progbits
	.align	2
	.globl	main
	.type	main, @function
main:
	lui	a5,%hi(.LANCHOR0)
	addi	a5,a5,%lo(.LANCHOR0)
	lw	a4,0(a5)
	addi	sp,sp,-80
	lw	t2,4(a5)
	lw	t0,8(a5)
	lw	t6,12(a5)
	lw	t5,16(a5)
	lw	t4,20(a5)
	lw	t3,24(a5)
	lw	t1,28(a5)
	lw	a7,32(a5)
	lw	a6,36(a5)
	lw	a2,40(a5)
	lw	a3,44(a5)
	sw	a4,8(sp)
	lw	a4,48(a5)
	lw	a5,52(a5)
	li	a1,7
	addi	a0,sp,8
	sw	a4,56(sp)
	sw	a5,60(sp)
	sw	ra,76(sp)
	sw	t2,12(sp)
	sw	t0,16(sp)
	sw	t6,20(sp)
	sw	t5,24(sp)
	sw	t4,28(sp)
	sw	t3,32(sp)
	sw	t1,36(sp)
	sw	a7,40(sp)
	sw	a6,44(sp)
	sw	a2,48(sp)
	sw	a3,52(sp)
	call	bubbleSort
	addi	a4,sp,36
	addi	a5,sp,8
	mv	a1,a4
.L14:
	lw	a2,0(a5)
	lw	a3,0(a4)
	addi	a5,a5,4
	addi	a4,a4,4
	bne	a2,a3,.L15
	bne	a5,a1,.L14
	li	a0,0
.L12:
	lw	ra,76(sp)
	addi	sp,sp,80
	jr	ra
.L15:
	li	a0,-1
	j	.L12
	.size	main, .-main
	.section	.rodata
	.align	2
	.set	.LANCHOR0,. + 0
.LC0:
	.word	64
	.word	34
	.word	25
	.word	12
	.word	22
	.word	11
	.word	90
.LC1:
	.word	11
	.word	12
	.word	22
	.word	25
	.word	34
	.word	64
	.word	90
	.ident	"GCC: (GNU) 7.2.0"
