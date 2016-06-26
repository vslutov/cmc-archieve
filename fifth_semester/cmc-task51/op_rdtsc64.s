	.text
	.globl	op_rdtsc
	.type	op_rdtsc, @function
op_rdtsc:
    rdtsc
    shlq    $32, %rdx
    orq     %rdx, %rax
	ret
