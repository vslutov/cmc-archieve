# Ввести натуральное N, затем N целых чисел ai; посчитать формулу a0-a1+a2-…±aN-1 . 
# Вывести результат.

	# t0 = read integer
	li	$v0 5
	syscall
	move	$t0 $v0
	
	# t2 = 0
	li 	$t2 0
	# t3 = 1
	li	$t3 1
	
	# for t1 in [0..t0)
	li 	$t1 0
	j	.eq
.inc:
	add	$t1 $t1 1
.eq:
	beq	$t1 $t0 .endfor

	#   v0 = read integer
	li	$v0 5
	syscall
	
	#   t2 += v0 * t3
	mul	$v0 $v0 $t3
	add	$t2 $t2 $v0
	#   t3 *= -1
	neg	$t3 $t3

	j	.inc
.endfor:
	
	# print t2
	li	$v0 1
	move	$a0 $t2
	syscall
	
	li	$v0 10
	syscall