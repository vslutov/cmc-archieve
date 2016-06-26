	.text
	.globl	op_rdtsc
	.type	op_rdtsc, @function
op_rdtsc:
    rdtsc
	ret
