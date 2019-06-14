#include "data.h"

/* load an FFT data file into memory */
DataSet loadData ( unsigned int dataFile )
{
	DataSet data;

	FILE *file;

	/* select the data file based on config param */
	switch ( dataFile )
	{
		case 0:
			file = fopen( FILE1, "r" );
		break;
		case 1:
			file = fopen( FILE2, "r" );
		break;
		case 2:
			file = fopen( FILE3, "r" );
		break;
		case 3:
			file = fopen( FILE4, "r" );
		break;
		default:
			exit( 1 );
		break;
	}

	/* read the size and dimensionality of vectors and alloc memory */
	fscanf( file, "%d %d", &data.size, &data.dim );

	data.vector = malloc( data.size * sizeof( DataVector ) );
	if ( !data.vector )
	{
		printf( "%s\n", CANT_ALLOC );
		exit( 1 );
	}
	
	data.max = malloc( data.dim * sizeof( float ) );
	if ( !data.max )
	{
		printf( "%s\n", CANT_ALLOC );
		exit( 1 );
	}

	unsigned int i;
	for ( i = 0; i < data.dim; i++ )
		data.max[ i ] = 0;

	unsigned int j;
	for ( i = 0; i < data.size; i++ )
	{
		/* load predetermined classification */
		fscanf( file, "%d", &data.vector[ i ].type );
		data.vector[ i ].testData = 0;

		/* alloc for data vector... */
		data.vector[ i ].value = malloc( data.dim * sizeof( float ) );
		if ( !data.vector[ i ].value )
		{
			printf( "%s\n", CANT_ALLOC );
			exit( 1 );
		}

		/* ...and read in data, recording max values */
		for ( j = 0; j < data.dim; j++ )
		{
			fscanf( file, "%f", &data.vector[ i ].value[ j ] );
			if ( data.vector[ i ].value[ j ] > data.max[ j ] )
				data.max[ j ] = data.vector[ i ].value[ j ];
		}
	}

	fclose( file );

	return data;
}

/* print data for debug */
void printData ( DataSet *data )
{
	printf( "%d %d\n", ( *data ).size, ( *data ).dim );

	unsigned int i, j;
	for ( i = 0; i < ( *data ).size; i++ )
	{
		printf( "[%d] %d\n", i, ( *data ).vector[ i ].type );

		for ( j = 0; j < ( *data ).dim; j++ )
			printf( "[%d] %4.2f ", j,
				( *data ).vector[ i ].value[ j ] );
		
		printf( "\n\n" );
	}
	
	printf( "Max\n" );
	for ( i = 0; i < ( *data ).dim; i++ )
		printf( "[%d] %4.2f ", i, ( *data ).max[ i ] );

	printf( "\n\n" );
}

/* free data set from memory */
void freeData ( DataSet *data )
{
	if ( ( *data ).vector )
	{
		unsigned int i;
		for ( i = 0; i < ( *data ).size; i++ )
		{
			if ( ( *data ).vector[ i ].value )
			{
				free( ( *data ).vector[ i ].value );
				( *data ).vector[ i ].value = NULL;
			}
		}
		free( ( *data ).vector );
		( *data ).vector = NULL;
	}

	if ( ( *data ).max )
	{
		free( ( *data ).max );
		( *data ).max = NULL;
	}
}
