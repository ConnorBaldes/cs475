#include <iostream>
#include <cstdlib>
#include <xmmintrin.h>
#include<omp.h>
#include <math.h>
#include <fstream>
using namespace std;

#ifndef NUMT
#define NUMT 1
#endif

#ifndef EC
#define EC 0
#endif

#ifndef NUMTRIES
#define NUMTRIES	80
#endif

#ifndef ARSIZE
#define ARSIZE	1000
#endif

#define NUM_ELEMENTS_PER_CORE ARSIZE/NUMT

#define SSE_WIDTH		4

void SimdMul( float *a, float *b,   float *c,   int len ) {
	int limit = ( len/SSE_WIDTH ) * SSE_WIDTH;
	register float *pa = a;
	register float *pb = b;
	register float *pc = c;
	for( int i = 0; i < limit; i += SSE_WIDTH )
	{
		_mm_storeu_ps( pc,  _mm_mul_ps( _mm_loadu_ps( pa ), _mm_loadu_ps( pb ) ) );
		pa += SSE_WIDTH;
		pb += SSE_WIDTH;
		pc += SSE_WIDTH;
	}

	for( int i = limit; i < len; i++ )
	{
		c[i] = a[i] * b[i];
	}
}


float SimdMulSum( float *a, float *b, int len ) {
	float sum[4] = { 0., 0., 0., 0. };
	int limit = ( len/SSE_WIDTH ) * SSE_WIDTH;
	register float *pa = a;
	register float *pb = b;

	__m128 ss = _mm_loadu_ps( &sum[0] );
	for( int i = 0; i < limit; i += SSE_WIDTH )
	{
		ss = _mm_add_ps( ss, _mm_mul_ps( _mm_loadu_ps( pa ), _mm_loadu_ps( pb ) ) );
		pa += SSE_WIDTH;
		pb += SSE_WIDTH;
	}
	_mm_storeu_ps( &sum[0], ss );

	for( int i = limit; i < len; i++ )
	{
		sum[0] += a[i] * b[i];
	}

	return sum[0] + sum[1] + sum[2] + sum[3];
}

void cppmul( float *a, float *b,   float *c,   int len ) {


	for(int i = 0; i < len; i++ ) {

		c[i] = a[i] * b[i];
	}
}

float cppmulsum( float *a, float *b, int len ) {

    float sum = 0;

    for( int i = 0; i < len; i++ ) {
		sum += a[i] * b[i];
	}

	return sum;
}



int main() {

    double maxPerformancesimd = 0.;
    double maxPerformancecpp = 0.;
    float* ar1;
    float* ar2;
    float* ar3;
    ar1 = new float[ARSIZE];
    ar2 = new float[ARSIZE];
    ar3 = new float[ARSIZE];
    float sum = 0;
    if(EC == 0) {

        for( int tries = 0; tries < NUMTRIES; tries++ ) {

            double time0 = omp_get_wtime( );

            SimdMul(ar1, ar2, ar3, ARSIZE);
            sum = SimdMulSum(ar1, ar2, ARSIZE);

            double time1 = omp_get_wtime( );
	        double megaHeightsPerSecond = ( time1 - time0 ) / 1000000.;
	        if( megaHeightsPerSecond > maxPerformancesimd )
		        maxPerformancesimd = megaHeightsPerSecond;
        }


        for( int tries = 0; tries < NUMTRIES; tries++ ) {

            double time0 = omp_get_wtime( );

            cppmul(ar1, ar2, ar3, ARSIZE);
            sum = cppmulsum(ar1, ar2, ARSIZE);

            double time1 = omp_get_wtime( );
	        double megaHeightsPerSecond = ( time1 - time0 ) / 1000000.;
	        if( megaHeightsPerSecond > maxPerformancecpp )
		        maxPerformancecpp = megaHeightsPerSecond;
        }
        cout << ARSIZE <<" "<< maxPerformancesimd <<" "<< maxPerformancecpp << endl;

    }
    else if(EC == 1){
        
        omp_set_num_threads( NUMT );
        double maxMegaMultsPerSecond = 0.;
        for( int t = 0; t < NUMTRIES; t++ )
        {
            double time0 = omp_get_wtime( );
            #pragma omp parallel
            {
                int first = omp_get_thread_num( ) * NUM_ELEMENTS_PER_CORE;
                SimdMul( &ar1[first], &ar2[first], &ar3[first], NUM_ELEMENTS_PER_CORE );
            }
            double time1 = omp_get_wtime( );
            double megaMultsPerSecond = (float)ARSIZE / ( time1 - time0 ) / 1000000.;
            if( megaMultsPerSecond > maxMegaMultsPerSecond )
                maxMegaMultsPerSecond = megaMultsPerSecond;
        }
        cout <<" "<< NUMT <<" "<< ARSIZE <<" "<< maxMegaMultsPerSecond << endl;
        
    } 
 


    

    return 0;
}