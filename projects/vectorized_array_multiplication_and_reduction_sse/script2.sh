#!/bin/tcsh
#number of threads:
foreach t (1 2 4 8 12 16)
    #array sizes
    foreach s (1000 10000 100000 500000 1000000 4000000 8000000)
        g++ -DEC=1 -DNUMT=$t -DARSIZE=$s driver.cpp -o ./driver -lm -fopenmp
        ./driver
    end
end