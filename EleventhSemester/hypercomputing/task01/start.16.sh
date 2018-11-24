#!/usr/bin/bash
for i in {7512..64000..2000}
do
  OMP_NUM_THREADS=2 mpiexec -n 16 ./task01 ${i}
done
