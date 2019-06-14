#include "math.h"

/* gaussian function for normal distribution curve, from wikipedia */
float gauss ( float x, float a, float b, float c )
{
	float num = ( x - b ) * ( x - b );
	float denom = 2 * c * c;
	return ( a * exp( -( num / denom ) ) );
}

/* ricker wavelet, aka mexican hat function, from wikipedia */
float ricker ( float x, float a )
{
	float first = 2 / ( sqrt( 3 * a ) * pow( acos( -1 ), 0.25 ) );
	float second = 1 - ( ( x * x ) / ( a * a ) );
	float third = exp( ( -( x * x ) ) / ( 2 * ( a * a ) ) );
	return ( first * second * third );
}

/* exponential decay, also from wikipedia */
float decay ( float x, float i, float d )
{
	return ( i * exp( -d * x ));
}

/* euclidian distance for n sized vectors, from AIJunkie tutorial */
float euclid ( float *v1, float *v2, unsigned int n )
{
	float dist = 0;

	unsigned int i;
	for ( i = 0; i < n; i++ )
		dist += ( v1[ i ] - v2[ i ] ) * ( v1[ i ] - v2[ i ] );

	return sqrt( dist );
}

/* return a random float from min to max */
float randFlt ( float min, float max )
{
        if ( min >= max )
                return 0.0;

        return ( (float) rand() / RAND_MAX ) * ( max - min ) + min;
}

/* simple comparison function, priority on x */
float compFlts ( float x, float y, int larger )
{
	if ( larger )
		if ( x >= y )
			return x;
		else
			return y;
	else
		if ( x <= y )
			return x;
		else
			return y;
}
