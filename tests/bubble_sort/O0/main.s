	.file	"main.c"
	.option nopic
	.text
	.align	2
	.globl	swap
	.type	swap, @function
swap:
	addi	sp,sp,-48
	sw	s0,44(sp)
	addi	s0,sp,48
	sw	a0,-36(s0)
	sw	a1,-40(s0)
	lw	a5,-36(s0)
	lw	a5,0(a5)
	sw	a5,-20(s0)
	lw	a5,-40(s0)
	lw	a4,0(a5)
	lw	a5,-36(s0)
	sw	a4,0(a5)
	lw	a5,-40(s0)
	lw	a4,-20(s0)
	sw	a4,0(a5)
	nop
	lw	s0,44(sp)
	addi	sp,sp,48
	jr	ra
	.size	swap, .-swap
	.align	2
	.globl	bubbleSort
	.type	bubbleSort, @function
bubbleSort:
	addi	sp,sp,-48
	sw	ra,44(sp)
	sw	s0,40(sp)
	addi	s0,sp,48
	sw	a0,-36(s0)
	sw	a1,-40(s0)
	sw	zero,-20(s0)
	j	.L3
.L7:
	sw	zero,-24(s0)
	j	.L4
.L6:
	lw	a5,-24(s0)
	slli	a5,a5,2
	lw	a4,-36(s0)
	add	a5,a4,a5
	lw	a4,0(a5)
	lw	a5,-24(s0)
	addi	a5,a5,1
	slli	a5,a5,2
	lw	a3,-36(s0)
	add	a5,a3,a5
	lw	a5,0(a5)
	ble	a4,a5,.L5
	lw	a5,-24(s0)
	slli	a5,a5,2
	lw	a4,-36(s0)
	add	a3,a4,a5
	lw	a5,-24(s0)
	addi	a5,a5,1
	slli	a5,a5,2
	lw	a4,-36(s0)
	add	a5,a4,a5
	mv	a1,a5
	mv	a0,a3
	call	swap
.L5:
	lw	a5,-24(s0)
	addi	a5,a5,1
	sw	a5,-24(s0)
.L4:
	lw	a4,-40(s0)
	lw	a5,-20(s0)
	sub	a5,a4,a5
	addi	a5,a5,-1
	lw	a4,-24(s0)
	blt	a4,a5,.L6
	lw	a5,-20(s0)
	addi	a5,a5,1
	sw	a5,-20(s0)
.L3:
	lw	a5,-40(s0)
	addi	a5,a5,-1
	lw	a4,-20(s0)
	blt	a4,a5,.L7
	nop
	lw	ra,44(sp)
	lw	s0,40(sp)
	addi	sp,sp,48
	jr	ra
	.size	bubbleSort, .-bubbleSort
	.section	.rodata
	.align	2
.LC0:
	.word	64
	.word	34
	.word	25
	.word	12
	.word	22
	.word	11
	.word	90
	.align	2
.LC1:
	.word	11
	.word	12
	.word	22
	.word	25
	.word	34
	.word	64
	.word	90
	.text
	.align	2
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-80
	sw	ra,76(sp)
	sw	s0,72(sp)
	addi	s0,sp,80
	lui	a5,%hi(.LC0)
	lw	a6,%lo(.LC0)(a5)
	addi	a4,a5,%lo(.LC0)
	lw	a0,4(a4)
	addi	a4,a5,%lo(.LC0)
	lw	a1,8(a4)
	addi	a4,a5,%lo(.LC0)
	lw	a2,12(a4)
	addi	a4,a5,%lo(.LC0)
	lw	a3,16(a4)
	addi	a4,a5,%lo(.LC0)
	lw	a4,20(a4)
	addi	a5,a5,%lo(.LC0)
	lw	a5,24(a5)
	sw	a6,-52(s0)
	sw	a0,-48(s0)
	sw	a1,-44(s0)
	sw	a2,-40(s0)
	sw	a3,-36(s0)
	sw	a4,-32(s0)
	sw	a5,-28(s0)
	lui	a5,%hi(.LC1)
	lw	a6,%lo(.LC1)(a5)
	addi	a4,a5,%lo(.LC1)
	lw	a0,4(a4)
	addi	a4,a5,%lo(.LC1)
	lw	a1,8(a4)
	addi	a4,a5,%lo(.LC1)
	lw	a2,12(a4)
	addi	a4,a5,%lo(.LC1)
	lw	a3,16(a4)
	addi	a4,a5,%lo(.LC1)
	lw	a4,20(a4)
	addi	a5,a5,%lo(.LC1)
	lw	a5,24(a5)
	sw	a6,-80(s0)
	sw	a0,-76(s0)
	sw	a1,-72(s0)
	sw	a2,-68(s0)
	sw	a3,-64(s0)
	sw	a4,-60(s0)
	sw	a5,-56(s0)
	li	a5,7
	sw	a5,-24(s0)
	addi	a5,s0,-52
	lw	a1,-24(s0)
	mv	a0,a5
	call	bubbleSort
	sw	zero,-20(s0)
	j	.L9
.L12:
	lw	a5,-20(s0)
	slli	a5,a5,2
	addi	a4,s0,-16
	add	a5,a4,a5
	lw	a4,-36(a5)
	lw	a5,-20(s0)
	slli	a5,a5,2
	addi	a3,s0,-16
	add	a5,a3,a5
	lw	a5,-64(a5)
	beq	a4,a5,.L10
	li	a5,-1
	j	.L13
.L10:
	lw	a5,-20(s0)
	addi	a5,a5,1
	sw	a5,-20(s0)
.L9:
	lw	a4,-20(s0)
	lw	a5,-24(s0)
	blt	a4,a5,.L12
	li	a5,0
.L13:
	mv	a0,a5
	lw	ra,76(sp)
	lw	s0,72(sp)
	addi	sp,sp,80
	jr	ra
	.size	main, .-main
	.ident	"GCC: (GNU) 7.2.0"
