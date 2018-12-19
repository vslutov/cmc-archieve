#!/usr/bin/env sh
make -f Makefile.mpi-blue Makefile.mpimp-blue

for p in 128 256 512 125 216
do
  for m in 512 1024 1536
  do
    mpisubmit.bg -n $p -w 00:05:00 parmpi.$m
    mpisubmit.bg -n $p -w 00:05:00 -e "OMP_NUM_THREADS=3" parmpimp.$m
  done
done
