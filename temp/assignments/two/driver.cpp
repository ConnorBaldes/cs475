
#include<iostream>
#include<cstdlib>
#include<omp.h>
#include <math.h>
using namespace std;
#ifndef NUMT
#define NUMT 1
#endif

#ifndef NUMNODES
#define NUMNODES 64
#endif

#define XMIN     -1.
#define XMAX      1.
#define YMIN     -1.
#define YMAX      1.

#ifndef NUMTRIES
#define NUMTRIES	80
#endif
#define N	0.70

float Height( int, int );	// function prototype

int main( int argc, char *argv[ ] ) {
	omp_set_num_threads(NUMT);
	double maxPerformance = 0.;
	// the area of a single full-sized tile:
	// (not all tiles are full-sized, though)
	float fullTileArea = ((( XMAX - XMIN )/(float)(NUMNODES-1))*(( YMAX - YMIN )/(float)(NUMNODES-1)));
    float halfTileArea = (((( XMAX - XMIN )/(float)(NUMNODES-1))*(( YMAX - YMIN )/(float)(NUMNODES-1)))/2);
    float quarterTileArea = (((( XMAX - XMIN )/(float)(NUMNODES-1))*(( YMAX - YMIN )/(float)(NUMNODES-1)))/4);
	// sum up the weighted heights into the variable "volume"
	// using an OpenMP for loop and a reduction:
    float volume;
    for( int tries = 0; tries < NUMTRIES; tries++ ) {
        volume = 0;
        double time0 = omp_get_wtime( );
        #pragma omp parallel for default(none) shared(fullTileArea, halfTileArea, quarterTileArea) reduction(+:volume)
        for( int i = 0; i < NUMNODES*NUMNODES; i++ ) {
	        int iu = i % NUMNODES;
	        int iv = i / NUMNODES;
        
	        float z = Height( iu, iv );
            if((iu == 0 && iv == 0) || (iu == 0 && iv == NUMNODES-1) || (iu == NUMNODES-1 && iv == 0) || (iu == NUMNODES-1 && iv == NUMNODES-1)) {
                z = z*quarterTileArea;
            }
	        else if(iu == 0 || iv == 0 || iu == NUMNODES-1 || iv == NUMNODES-1) {
                z = z*halfTileArea;
            }
            else {
                z = z*fullTileArea;
            }
            volume += z;
        }
        double time1 = omp_get_wtime( );
	    double megaHeightsPerSecond = (double)NUMNODES / ( time1 - time0 ) / 1000000.;
	    if( megaHeightsPerSecond > maxPerformance )
		    maxPerformance = megaHeightsPerSecond;
    }
    volume = volume*2;
    cout << NUMT << "            " << NUMNODES << "         " << volume << "      " << maxPerformance << endl;
    return 0;
}

float Height( int iu, int iv ){	// iu,iv = 0 .. NUMNODES-1

	float x = -1.  +  2.*(float)iu /(float)(NUMNODES-1);	// -1. to +1.
	float y = -1.  +  2.*(float)iv /(float)(NUMNODES-1);	// -1. to +1.

	float xn = pow( fabs(x), (double)N );
	float yn = pow( fabs(y), (double)N );
	float r = 1. - xn - yn;
	if( r <= 0. )
	        return 0.;
	float height = pow( r, 1./(float)N );
	return height;
}