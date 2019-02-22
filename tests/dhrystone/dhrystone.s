	.file	"dhrystone.c"
	.option nopic
	.text
	.align	2
	.globl	Proc2
	.type	Proc2, @function
Proc2:
	lui	a5,%hi(Char1Glob)
	lbu	a4,%lo(Char1Glob)(a5)
	li	a5,65
	beq	a4,a5,.L4
	ret
.L4:
	lw	a5,0(a0)
	lui	a4,%hi(IntGlob)
	lw	a4,%lo(IntGlob)(a4)
	addi	a5,a5,9
	sub	a5,a5,a4
	sw	a5,0(a0)
	ret
	.size	Proc2, .-Proc2
	.align	2
	.globl	Proc4
	.type	Proc4, @function
Proc4:
	lui	a5,%hi(Char2Glob)
	li	a4,66
	sb	a4,%lo(Char2Glob)(a5)
	ret
	.size	Proc4, .-Proc4
	.align	2
	.globl	Proc5
	.type	Proc5, @function
Proc5:
	lui	a5,%hi(Char1Glob)
	li	a4,65
	sb	a4,%lo(Char1Glob)(a5)
	lui	a5,%hi(BoolGlob)
	sw	zero,%lo(BoolGlob)(a5)
	ret
	.size	Proc5, .-Proc5
	.align	2
	.globl	Proc7
	.type	Proc7, @function
Proc7:
	addi	a0,a0,2
	add	a1,a0,a1
	sw	a1,0(a2)
	ret
	.size	Proc7, .-Proc7
	.align	2
	.globl	Proc3
	.type	Proc3, @function
Proc3:
	lui	a5,%hi(PtrGlb)
	lw	a2,%lo(PtrGlb)(a5)
	beqz	a2,.L9
	lw	a4,0(a2)
	lui	a3,%hi(IntGlob)
	lw	a1,%lo(IntGlob)(a3)
	sw	a4,0(a0)
	lw	a2,%lo(PtrGlb)(a5)
	li	a0,10
	addi	a2,a2,12
	tail	Proc7
.L9:
	lui	a5,%hi(IntGlob)
	li	a4,100
	li	a1,100
	addi	a2,a2,12
	li	a0,10
	sw	a4,%lo(IntGlob)(a5)
	tail	Proc7
	.size	Proc3, .-Proc3
	.align	2
	.globl	Proc8
	.type	Proc8, @function
Proc8:
	addi	a4,a2,5
	li	a6,204
	mul	a6,a4,a6
	slli	a2,a2,2
	slli	a5,a4,2
	add	a0,a0,a5
	sw	a3,0(a0)
	sw	a4,120(a0)
	sw	a3,4(a0)
	add	a5,a6,a2
	add	a5,a1,a5
	lw	a3,16(a5)
	sw	a4,20(a5)
	sw	a4,24(a5)
	addi	a4,a3,1
	sw	a4,16(a5)
	lw	a5,0(a0)
	add	a1,a1,a6
	add	a1,a1,a2
	li	a2,4096
	add	a1,a2,a1
	sw	a5,4(a1)
	li	a4,5
	lui	a5,%hi(IntGlob)
	sw	a4,%lo(IntGlob)(a5)
	ret
	.size	Proc8, .-Proc8
	.align	2
	.globl	Func1
	.type	Func1, @function
Func1:
	sub	a0,a0,a1
	seqz	a0,a0
	ret
	.size	Func1, .-Func1
	.align	2
	.globl	Func2
	.type	Func2, @function
Func2:
	addi	sp,sp,-16
	sw	s0,8(sp)
	sw	s1,4(sp)
	sw	ra,12(sp)
	mv	s0,a0
	mv	s1,a1
	lbu	a0,1(a0)
	lbu	a1,2(a1)
	call	Func1
.L14:
	bnez	a0,.L14
	mv	a1,s1
	mv	a0,s0
	call	strcmp
	lw	ra,12(sp)
	lw	s0,8(sp)
	lw	s1,4(sp)
	sgt	a0,a0,zero
	addi	sp,sp,16
	jr	ra
	.size	Func2, .-Func2
	.align	2
	.globl	Func3
	.type	Func3, @function
Func3:
	addi	a0,a0,-2
	seqz	a0,a0
	ret
	.size	Func3, .-Func3
	.align	2
	.globl	Proc6
	.type	Proc6, @function
Proc6:
	addi	sp,sp,-16
	sw	s0,8(sp)
	sw	s1,4(sp)
	sw	ra,12(sp)
	mv	s0,a0
	mv	s1,a1
	call	Func3
	beqz	a0,.L19
	sw	s0,0(s1)
	li	a4,1
	beq	s0,a4,.L21
.L29:
	beqz	s0,.L28
	li	a5,2
	beq	s0,a5,.L23
	li	a4,4
	bne	s0,a4,.L18
	sw	a5,0(s1)
.L18:
	lw	ra,12(sp)
	lw	s0,8(sp)
	lw	s1,4(sp)
	addi	sp,sp,16
	jr	ra
.L19:
	li	a5,3
	sw	a5,0(s1)
	li	a4,1
	bne	s0,a4,.L29
.L21:
	lui	a5,%hi(IntGlob)
	lw	a4,%lo(IntGlob)(a5)
	li	a5,100
	ble	a4,a5,.L26
.L28:
	lw	ra,12(sp)
	lw	s0,8(sp)
	sw	zero,0(s1)
	lw	s1,4(sp)
	addi	sp,sp,16
	jr	ra
.L23:
	lw	ra,12(sp)
	lw	s0,8(sp)
	sw	a4,0(s1)
	lw	s1,4(sp)
	addi	sp,sp,16
	jr	ra
.L26:
	li	a5,3
	sw	a5,0(s1)
	j	.L18
	.size	Proc6, .-Proc6
	.align	2
	.globl	Proc1
	.type	Proc1, @function
Proc1:
	addi	sp,sp,-16
	sw	s1,4(sp)
	lui	s1,%hi(PtrGlb)
	lw	a4,%lo(PtrGlb)(s1)
	lw	a5,0(a0)
	sw	ra,12(sp)
	lw	t0,0(a4)
	sw	s0,8(sp)
	lw	t6,4(a4)
	lw	t5,8(a4)
	lw	t4,12(a4)
	lw	t3,16(a4)
	lw	t1,20(a4)
	lw	a7,24(a4)
	lw	a6,28(a4)
	lw	a1,36(a4)
	lw	a2,40(a4)
	lw	a3,44(a4)
	mv	s0,a0
	lw	a0,32(a4)
	sw	t0,0(a5)
	lw	a4,0(s0)
	sw	t6,4(a5)
	sw	t5,8(a5)
	sw	t4,12(a5)
	sw	t3,16(a5)
	sw	t1,20(a5)
	sw	a7,24(a5)
	sw	a6,28(a5)
	sw	a1,36(a5)
	sw	a2,40(a5)
	sw	a3,44(a5)
	sw	a0,32(a5)
	li	a5,5
	sw	a5,12(s0)
	sw	a4,0(a4)
	lw	a0,0(s0)
	sw	a5,12(a4)
	call	Proc3
	lw	a5,0(s0)
	lw	a4,4(a5)
	beqz	a4,.L33
	lw	t5,0(a5)
	lw	t4,8(a5)
	lw	t3,12(a5)
	lw	t1,16(a5)
	lw	a7,20(a5)
	lw	a6,24(a5)
	lw	a0,28(a5)
	lw	a1,32(a5)
	lw	a2,36(a5)
	lw	a3,40(a5)
	lw	a5,44(a5)
	lw	ra,12(sp)
	sw	t5,0(s0)
	sw	a4,4(s0)
	sw	t4,8(s0)
	sw	t3,12(s0)
	sw	t1,16(s0)
	sw	a7,20(s0)
	sw	a6,24(s0)
	sw	a0,28(s0)
	sw	a1,32(s0)
	sw	a2,36(s0)
	sw	a3,40(s0)
	sw	a5,44(s0)
	lw	s0,8(sp)
	lw	s1,4(sp)
	addi	sp,sp,16
	jr	ra
.L33:
	lw	a0,8(s0)
	li	a4,6
	addi	a1,a5,8
	sw	a4,12(a5)
	call	Proc6
	lw	a4,%lo(PtrGlb)(s1)
	lw	a5,0(s0)
	lw	ra,12(sp)
	lw	a4,0(a4)
	lw	s1,4(sp)
	li	a1,10
	sw	a4,0(a5)
	lw	a2,0(s0)
	lw	s0,8(sp)
	lw	a0,12(a2)
	addi	a2,a2,12
	addi	sp,sp,16
	tail	Proc7
	.size	Proc1, .-Proc1
	.align	2
	.globl	Proc0
	.type	Proc0, @function
Proc0:
	addi	sp,sp,-176
	li	a0,48
	sw	ra,172(sp)
	sw	s0,168(sp)
	sw	s1,164(sp)
	sw	s2,160(sp)
	sw	s3,156(sp)
	sw	s4,152(sp)
	sw	s5,148(sp)
	sw	s6,144(sp)
	sw	s7,140(sp)
	sw	s8,136(sp)
	sw	s9,132(sp)
	sw	s10,128(sp)
	sw	s11,124(sp)
	call	malloc
	mv	s0,a0
	lui	a5,%hi(PtrGlbNext)
	li	a0,48
	sw	s0,%lo(PtrGlbNext)(a5)
	call	malloc
	lui	a3,%hi(.LC0)
	addi	a5,a3,%lo(.LC0)
	lw	a3,%lo(.LC0)(a3)
	lw	t6,4(a5)
	lw	t5,8(a5)
	lw	t4,12(a5)
	lw	t3,16(a5)
	lw	t1,20(a5)
	lw	a7,24(a5)
	lhu	a6,28(a5)
	lbu	a2,30(a5)
	li	a5,2
	sw	a5,8(a0)
	li	a5,40
	mv	a4,a0
	sw	s0,0(a0)
	sw	a5,12(a0)
	sw	a3,16(a0)
	sw	zero,4(a0)
	lui	s5,%hi(Array2Glob)
	addi	a3,s5,%lo(Array2Glob)
	li	a5,10
	lui	s2,%hi(.LC1)
	lui	s4,%hi(PtrGlb)
	li	a1,0
	addi	a0,sp,16
	sw	t6,20(a4)
	sw	t5,24(a4)
	sw	t4,28(a4)
	sw	t3,32(a4)
	sw	t1,36(a4)
	sw	a7,40(a4)
	sh	a6,44(a4)
	sb	a2,46(a4)
	li	s3,9998336
	sw	a4,%lo(PtrGlb)(s4)
	sw	a5,1660(a3)
	addi	s3,s3,1664
	call	gettimeofday
	lui	s9,%hi(BoolGlob)
	lw	s10,%lo(.LC1)(s2)
	lui	s8,%hi(Array1Glob)
	addi	s2,s2,%lo(.LC1)
	lui	s1,%hi(Char2Glob)
	li	s0,2
	li	s7,1
	li	s6,64
.L39:
	call	Proc5
	call	Proc4
	lw	t3,4(s2)
	lw	t1,8(s2)
	lw	a7,12(s2)
	lw	a6,16(s2)
	lw	a2,20(s2)
	lw	a3,24(s2)
	lhu	a4,28(s2)
	lbu	a5,30(s2)
	addi	a1,sp,80
	addi	a0,sp,48
	sw	s0,4(sp)
	sw	s10,80(sp)
	sw	t3,84(sp)
	sw	t1,88(sp)
	sw	a7,92(sp)
	sw	a6,96(sp)
	sw	a2,100(sp)
	sw	a3,104(sp)
	sh	a4,108(sp)
	sb	a5,110(sp)
	sw	s7,12(sp)
	call	Func2
	seqz	a0,a0
	sw	a0,%lo(BoolGlob)(s9)
	li	a0,2
.L35:
	slli	a5,a0,2
	add	a5,a5,a0
	addi	a5,a5,-3
	addi	a2,sp,8
	li	a1,3
	sw	a5,8(sp)
	call	Proc7
	lw	a0,4(sp)
	addi	a0,a0,1
	sw	a0,4(sp)
	ble	a0,s0,.L35
	lw	a3,8(sp)
	mv	a2,a0
	addi	a1,s5,%lo(Array2Glob)
	addi	a0,s8,%lo(Array1Glob)
	call	Proc8
	lw	a0,%lo(PtrGlb)(s4)
	call	Proc1
	lbu	a5,%lo(Char2Glob)(s1)
	bleu	a5,s6,.L36
	li	s11,65
	j	.L38
.L37:
	lbu	a5,%lo(Char2Glob)(s1)
	addi	s11,s11,1
	andi	s11,s11,0xff
	bltu	a5,s11,.L36
.L38:
	li	a1,67
	mv	a0,s11
	call	Func1
	lw	a5,12(sp)
	bne	a0,a5,.L37
	addi	a1,sp,12
	li	a0,0
	call	Proc6
	lbu	a5,%lo(Char2Glob)(s1)
	addi	s11,s11,1
	andi	s11,s11,0xff
	bgeu	a5,s11,.L38
.L36:
	lw	a4,4(sp)
	addi	s3,s3,-1
	addi	a0,sp,4
	slli	a5,a4,1
	add	a5,a5,a4
	sw	a5,8(sp)
	call	Proc2
	bnez	s3,.L39
	li	a1,0
	addi	a0,sp,32
	call	gettimeofday
	lw	a5,32(sp)
	li	a4,999424
	addi	a4,a4,576
	mul	a5,a5,a4
	lw	a3,16(sp)
	li	a2,9998336
	lui	a1,%hi(Version)
	lui	a0,%hi(.LC2)
	addi	a2,a2,1664
	addi	a1,a1,%lo(Version)
	addi	a0,a0,%lo(.LC2)
	mul	a4,a3,a4
	lw	a3,24(sp)
	sub	a3,a5,a3
	lw	a5,40(sp)
	add	a3,a3,a5
	li	a5,710000640
	sub	a3,a3,a4
	srai	a4,a3,3
	addi	a5,a5,-640
	div	a4,a5,a4
	call	printf
	lw	ra,172(sp)
	lw	s0,168(sp)
	lw	s1,164(sp)
	lw	s2,160(sp)
	lw	s3,156(sp)
	lw	s4,152(sp)
	lw	s5,148(sp)
	lw	s6,144(sp)
	lw	s7,140(sp)
	lw	s8,136(sp)
	lw	s9,132(sp)
	lw	s10,128(sp)
	lw	s11,124(sp)
	addi	sp,sp,176
	jr	ra
	.size	Proc0, .-Proc0
	.section	.text.startup,"ax",@progbits
	.align	2
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-16
	sw	ra,12(sp)
	call	Proc0
	li	a0,0
	call	exit
	.size	main, .-main
	.comm	PtrGlbNext,4,4
	.comm	PtrGlb,4,4
	.comm	Array2Glob,10404,4
	.comm	Array1Glob,204,4
	.comm	Char2Glob,1,1
	.comm	Char1Glob,1,1
	.comm	BoolGlob,4,4
	.comm	IntGlob,4,4
	.globl	Version
	.section	.rodata.str1.4,"aMS",@progbits,1
	.align	2
.LC0:
	.string	"DHRYSTONE PROGRAM, SOME STRING"
	.zero	1
.LC2:
	.string	"Dhrystone(%s), %ld passes, %ld microseconds, %ld DMIPS\n"
.LC1:
	.string	"DHRYSTONE PROGRAM, 2'ND STRING"
	.section	.sdata,"aw"
	.align	2
	.type	Version, @object
	.size	Version, 7
Version:
	.string	"1.1-mc"
	.ident	"GCC: (GNU) 8.2.0"
