#!/bin/bash
#SBATCH -J montecarlo
#SBATCH -A cs475-575
#SBATCH -p class
#SBATCH --gres=gpu:1
#SBATCH -o montecarlo.out
#SBATCH -e montecarlo.err
#SBATCH --mail-type=BEGIN,END,FAIL
#SBATCH --mail-user=baldesc@oregonstate.edu
for t in 16 32 64 128 
do
    for s in 2048 4096 8192 16384 32768 65536 131072 262144 524288 1024000
    do
        /usr/local/apps/cuda/cuda-10.1/bin/nvcc -DBLOCKSIZE=$t -DNUMTRIALS=$s -o montecarlo montecarlo.cu
        ./montecarlo 
    done
done