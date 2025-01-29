#!/bin/csh
foreach t ( 16 32 64 128 )
    foreach s (2048 4096 8192 16384 32768 65536 131072 262144 524288 1024000)
        /usr/local/apps/cuda/cuda-10.1/bin/nvcc -DBLOCKSIZE=$t -DNUMTRIALS=$s -o montecarlo montecarlo.cu
        ./montecarlo 
    end
end