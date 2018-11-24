#!/usr/bin/bash
for i in {5512..64000..2000}
do
  OMP_NUM_THREADS=2 mpiexec -n 4 ./task01 ${i}
done
