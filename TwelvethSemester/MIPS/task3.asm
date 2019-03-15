# Ввести целое N, затем N целых чисел.
# Вывести из этих чисел только чётные, причём в обратном порядке 
# (стеком пользоваться запрещается :) )

.data:
array:

.text:
	# t0 = read integer
	li	$v0 5
	syscall
	move	$t0 $v0
	
	# t3 = 0 -- pointer to array head
	li	$t3 0
	
	# for t1 in [0..t0):
	li 	$t1 0
.startfor:
	beq	$t1 $t0 .endfor

	#   v0 = read integer
	li	$v0 5
	syscall
	
	#   if v0 % 2 == 0:
	rem	$t2 $v0 2
	bnez	$t2 .endif
	#     save v0 to .data[t3]
	sw	$v0 array($t3)
	#     t3 += 4
	addi	$t3 $t3 4
.endif:

	add	$t1 $t1 1

	j	.startfor
.endfor:

	# while t3 != 0
	beqz	$t3 .endwhile
	
	#   t3 -= 4
	subi	$t3 $t3 4
		
	#   print .data($t3)
	li	$v0 1
	lw	$a0 array($t3)
	syscall
	
	#   print \n
	li	$v0 11
	li	$a0 '\n'
	syscall
	

	j	.endfor
.endwhile:
	
	li	$v0 10
	syscall