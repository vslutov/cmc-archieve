# Ввести целое N, затем N целых чисел.
# Вывести эти числа, пропуская уже выведенные, если встретятся повторы.
.text:
	# t0 = read integer
	li	$v0 5
	syscall
	move	$t0 $v0
	
	move	$fp $sp
	
	# for t1 in [0..t0):
	li 	$t1 0
.startouterfor:
	beq	$t1 $t0 .endouterfor

	#   v0 = read integer
	li	$v0 5
	syscall
	
	#   for t2 in [fp..sp) step -4:
	move	$t2 $fp
.startinnerfor:
	beq	$t2 $sp .endinnerfor

	#     if (t2) == v0:
	lw	$t3 ($t2)
	bne	$v0 $t3 .endif
	
	#       continue outer for
	j	.incouterfor
.endif:
	subi	$t2 $t2 4
	j	.startinnerfor
.endinnerfor:

	#   save v0 to stack
	sw	$v0 ($sp)
	subi	$sp $sp 4
	
	#   print v0
	move 	$a0 $v0
	li	$v0 1
	syscall

	#   print \n
	li	$v0 11
	li	$a0 '\n'
	syscall
	
.incouterfor:
	add	$t1 $t1 1

	j	.startouterfor
.endouterfor:
	
	li	$v0 10
	syscall
