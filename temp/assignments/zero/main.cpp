#include <omp.h>
#include <stdio.h>
#include <math.h>
#include <cstdlib>


using namespace std;

int main(int argc, char**argv) {
    //initialize values for program
    int NUMT  =  4;
    int SIZE  =  16384;
    int NUMTRIES = 10;
    float A[SIZE];
    float B[SIZE];
    float C[SIZE];
    //get desired thread number for test from command line
    NUMT = atoi(argv[1]);
    #ifndef _OPENMP
        fprintf( stderr, "OpenMP is not supported here -- sorry.\n" );
        return 1;
    #endif
    omp_set_num_threads(NUMT);
    fprintf( stderr, "Using %d threads\n", NUMT );
    double maxmm = 0;
    //fill first two arrays
    for(int i = 0; i < SIZE; i++) {

        A[i] = 1.0;
        B[i] = 2.0;
    }
    //loop for number of tries
    for(int j = 0; j < NUMTRIES; j++) {
        //get start time
        double start_time = omp_get_wtime();
        //initiallize multithreading
        #pragma omp parallel for
        for(int i = 0; i < SIZE; i++) {
            C[i] = A[i] * B[i];

        }
        //get end time
        double end_time = omp_get_wtime();
        //get execution time
        double mm = (double)SIZE/(end_time - start_time)/1000000;

        if( mm > maxmm ) {

            maxmm = mm;
        }
    }
    printf( "Peak Performance = %8.2lf MegaMults/Sec\n", maxmm );
    return 0;
}