#!/usr/bin/env sh
make -f Makefile.mpi-polus

for p in 128 256 512 125 216
do
  for m in 512 1024 1536
  do
    mpisubmit.pl -p $p parmpi.$m
  done
done
