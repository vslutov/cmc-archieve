# Sum number digits

	# v0 = read integer
	li	$v0 5
	syscall
	
	# v0 = abs(v0)
	abs	$v0 $v0
	# a0 = 0
	li	$a0 0
	
	# while v0 != 0:
.startwhile:
	beqz	$v0 .endwhile
	#   v1 = v0 % 10
	rem	$v1 $v0 10
	#   a0 += v1
	add	$a0 $a0 $v1
	#   v0 /= 10
	div	$v0 $v0 10
	j	.startwhile
.endwhile:
	
	# print integer a0
	li	$v0 1
	syscall
	
	# exit
	li	$v0 10
	syscall