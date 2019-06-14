#include "shuffle.h"

/* create a sorted list from 0 to size - 1 */
unsigned int *createOrder ( unsigned int size )
{
	unsigned int *order = malloc( size * sizeof( int ) );
	if ( !order )
	{
		printf( "%s\n", CANT_ALLOC );
		exit( 1 );
	}
	
	unsigned int i;
	for ( i = 0; i < size; i++ )
		order[ i ] = i;
	
	return order;
}

/* shuffle the given list */
void shuffleOrder ( unsigned int **order, unsigned int size )
{		
	int j, temp;

	for ( int i = size - 1; i > 0; i-- )
	{
		j = (int) randFlt( 0, i );
		temp = ( *order )[ i ];
		( *order )[ i ] = ( *order )[ j ];
		( *order )[ j ] = temp;
	}	
}

/* print order for debug */
void printOrder ( unsigned int *order, unsigned int size )
{
	unsigned int i;
	for ( i = 0; i < size; i++ )
		printf( "%8d%8d\n", i, order[ i ] );
}

/* test order for debug */
void testOrder ( unsigned int size )
{
	unsigned int *order = createOrder( size );
	shuffleOrder( &order, size );
	printOrder( order, size );
	free( order );
}
