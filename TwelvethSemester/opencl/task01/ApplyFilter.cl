__kernel void ApplyFilter(__global int* output, __global int* input)
{
 // Index of the elements to add
 unsigned int n = get_global_id(0);
 output[n] = input[n] * 2;
}
