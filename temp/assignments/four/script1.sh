#!/bin/tcsh
foreach s (1000 10000 100000 500000 1000000 4000000 8000000)
    g++ -DEC=0 -DARSIZE=$s driver.cpp -o ./driver -lm -fopenmp
    ./driver
end