#!/bin/tcsh

#number of threads:
foreach t (1 2 4 8 12 16)
    echo "\nThreads      Nodes     Volume        Performance(meganodes/sec)"
    #number of trieals
    foreach s (4 8 16 32 64 128 364 728)
        g++ -DNUMT=$t -DNUMNODES=$s driver.cpp -o ./driver -lm -fopenmp
        ./driver
    end
end