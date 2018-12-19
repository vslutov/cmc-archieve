#!/usr/bin/env sh
make -f Makefile.mpi-polus

for p in 1 2 4 8 16 32 27 48
do
  for m in 128 256 512
  do
    mpisubmit.pl -p $p parmpi.$m
  done
done
