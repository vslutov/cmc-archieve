
start:
	li 	$v0 5
	syscall
	move 	$a0 $v0
	
	li 	$v0 5
	syscall
	move 	$a1 $v0
	
	jal 	gcd
	
	move 	$a0 $v0
	li 	$v0 1
	syscall
	
	li 	$v0 10
	syscall

gcd:
	sub 	$sp $sp 4
	sw	$ra ($sp)
	
	bnez	$a1 .rec

	move 	$v0 $a0
	j	.ret

.rec:
	div	$a0 $a1
	move	$a0 $a1
	mfhi	$a1
	
	jal gcd

.ret:
	lw	$ra ($sp)
	add	$sp $sp 4
	jr 	$ra