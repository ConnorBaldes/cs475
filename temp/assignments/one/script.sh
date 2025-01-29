#!/bin/tcsh

#number of threads:
foreach t (1 2 4 6 8)
    echo "Threads      Trials     Probability    Performance(megatrials/sec)"
    #number of trieals
    foreach s (5000 10000 50000 100000)
        g++ -DNUMT=$t -DNUMTRIALS=$s main.cpp -o main -lm -fopenmp
        ./main
    end
end