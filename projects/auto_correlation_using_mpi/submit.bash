#!/bin/bash
#SBATCH -J Heat
#SBATCH -A cs475-575
#SBATCH -p class
#SBATCH -N 8 # number of nodes
#SBATCH -n 8 # number of tasks
#SBATCH -o mpi.out
#SBATCH -e mpi.err
#SBATCH --mail-type=END,FAIL
#SBATCH --mail-user=baldesc@oregonstate.edu
module load openmpi/3.1
mpic++ mpi.cpp -o mpi -lm
mpiexec -mca btl self,tcp -np 32 mpi