#!/bin/csh

foreach s ( 1024 4096 32768 131072 524288 2097152 8388608 33554432 )
    foreach t ( 8 32 128 256 512 1024 )
        g++ -DNMB=$s -DLOCAL_SIZE=$t -w -o first first.cpp /usr/local/apps/cuda/cuda-10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp

        ./first
    end
    echo " "
end