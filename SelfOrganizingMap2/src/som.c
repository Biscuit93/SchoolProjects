#include "som.h"

/* allocate memory for som */
SelfOrganizingMap loadSOM ( Config conf, DataSet data )
{
	SelfOrganizingMap som;

	som.config = conf;
	
	srand( som.config.intParam[ SEED ] );

	som.dim = data.dim;
	som.radius = 0;
	som.learnRate = 0;
	som.progress = 0;

	som.weight = malloc( som.config.intParam[ NET_WIDTH ] 
		* sizeof( float* ) );
	if ( !som.weight )
	{
		printf( "%s\n", CANT_ALLOC );
		exit( 1 );
	}

	som.match = malloc( som.config.intParam[ NET_WIDTH ]
		* sizeof( int* ) );
	if ( !som.match )
	{
		printf( "%s\n", CANT_ALLOC );
		exit( 1 );
	}
	
	som.class = malloc( som.config.intParam[ NET_WIDTH ]
		* sizeof( int* ) );
	if ( !som.class )
	{
		printf( "%s\n", CANT_ALLOC );
		exit( 1 );
	}

	unsigned int i, j;

	for ( i = 0; i < som.config.intParam[ NET_WIDTH ]; i++ )
	{
		som.weight[ i ] = malloc( som.config.intParam[ NET_HEIGHT ] 
			* sizeof( float* ) );
		if ( !som.weight[ i ] )
		{
			printf( "%s\n", CANT_ALLOC );
			exit( 1 );
		}
		
		som.match[ i ] = malloc( som.config.intParam[ NET_HEIGHT ] 
			* sizeof( int ) );
		if ( !som.match[ i ] )
		{
			printf( "%s\n", CANT_ALLOC );
			exit( 1 );
		}

		som.class[ i ] = malloc( som.config.intParam[ NET_HEIGHT ] 
			* sizeof( int ) );
		if ( !som.class[ i ] )
		{
			printf( "%s\n", CANT_ALLOC );
			exit( 1 );
		}

		for ( j = 0; j < som.config.intParam[ NET_HEIGHT ]; j++ )
		{
			som.weight[ i ][ j ] = malloc( som.dim 
				* sizeof( float ) );
			if ( !som.weight[ i ][ j ] )
			{
				printf( "%s\n", CANT_ALLOC );
				exit( 1 );
			}
		}
	}

	initWeights( &som, &data );

	return som;
}

/* initialize the weights randomly, set class and match values to -1 */
void initWeights ( SelfOrganizingMap *som, DataSet *data )
{
	unsigned int i, j, k;

	for ( i = 0; i < ( *som ).config.intParam[ NET_WIDTH ]; i++ )
	{
		for ( j = 0; j < ( *som ).config.intParam[ NET_HEIGHT ]; j++ )
		{
			for ( k = 0; k < ( *som ).dim; k++ )
			{
				( *som ).weight[ i ][ j ][ k ] =
					randFlt( 0, ( *data ).max[ k ] );
				( *som ).match[ i ][ j ] = -1;
				( *som ).class[ i ][ j ] = -1;
			}
		}
	}
}

/* train the thing to do things */
void trainSOM ( SelfOrganizingMap *som, DataSet *data )
{
	/* set up order for data input */
	unsigned int *order = createOrder( ( *data ).size );
	unsigned int i, j, x, y;

	/* find data index to start withholding data */
	int trainSize = ( 1.0 - ( *som ).config.fltParam[ HOLDOUT ] ) 
		* (float) ( *data ).size;
	
	/* shuffle only training data */
	shuffleOrder( &order, ( *data ).size );

	/* mark testing data as such */
	for ( i = trainSize; i < ( *data ).size; i++ )
		( *data ).vector[ order[ i ] ].testData = 1;

	/* epoch loop */
	for ( i = 0; i < ( *som ).config.intParam[ MAX_EPOCH ]; i++ )
	{
		shuffleOrder( &order, trainSize );

		/* calculate radius and learn rate for current epoch */
		( *som ).radius = compFlts( decay( i, 
			( *som ).config.fltParam[ RADIUS_INIT ],
			( *som ).config.fltParam[ RADIUS_DECAY ] ),
			( *som ).config.fltParam[ RADIUS_MIN ], 1 );
		
		( *som ).learnRate = compFlts( decay( i, 
			( *som ).config.fltParam[ LEARN_INIT ],
			( *som ).config.fltParam[ LEARN_DECAY ] ),
			( *som ).config.fltParam[ LEARN_MIN ], 1 );

		/* feed the training data in randomly */
		for ( j = 0; j < trainSize; j++ )
		{
			/* skip if testing data, for any reason */
			if ( ( *data ).vector[ order[ j ] ].testData )
				continue;
			else
			{
				/* set input vector randomly */
				( *som ).input = 
					( *data ).vector[ order[ j ] ].value;
			
				/* vital calculations */
				findBMU( som, &x, &y );
				updateWeights( som, x, y, i );
			}
		}
		
		/* get input, classify nodes, and draw to screen */
		getInput();
		classifyNodes( som, data );
		draw( som, data, i );
	}

	/* classify the data vectors by matching to nodes */
	classifyData( som, data, order );

	free( order );
	order = NULL;

	/* infinite loop to allow examination of output, user can exit or
	 * restart at this point */
	while ( 1 )
	{
		getInput();
		draw( som, data, ( *som ).config.intParam[ MAX_EPOCH ] );
	}
}

/* exactly as advertised */
void findBMU ( SelfOrganizingMap *som, unsigned int *x, unsigned int *y )
{
	unsigned int i, j, bestX, bestY;
	float dist, closest = FLT_MAX;

	/* for every node in lattice, find the closest one to input vector */
	for ( i = 0; i < ( *som ).config.intParam[ NET_WIDTH ]; i++ )
	{
		for ( j = 0; j < ( *som ).config.intParam[ NET_HEIGHT ]; j++ )
		{
			dist = euclid( ( *som ).weight[ i ][ j ], 
				( *som ).input, ( *som ).dim );

			if ( dist < closest )
			{
				closest = dist;
				bestX = i;
				bestY = j;
			}
		}
	}

	/* write the coordinates of BMU */
	( *x ) = bestX;
	( *y ) = bestY;
}

/* update the weights of the lattice nodes */
void updateWeights ( SelfOrganizingMap *som, unsigned int x, unsigned int y,
	unsigned int t )
{
	unsigned int i, j;
	float bmu[ 2 ];
	float node[ 2 ];
	float dist;

	bmu[ 0 ] = x;
	bmu[ 1 ] = y;

	/* for every node in lattice, find the distance and update the weight */
	for ( i = 0; i < ( *som ).config.intParam[ NET_WIDTH ]; i++ )
	{
		for ( j = 0; j < ( *som ).config.intParam[ NET_HEIGHT ]; j++ )
		{
			node[ 0 ] = i;
			node[ 1 ] = j;

			dist = findWrapDist( som, bmu, node );
			if ( !( ( *som ).config.intParam[ NET_CUTOFF ]
				&& !( dist <= ( *som ).radius ) ) )
				updateVector( som, i, j, dist );
		}
	}
}

/* update a single vector */
void updateVector ( SelfOrganizingMap *som, unsigned int x, unsigned int y,
	float dist )
{
	unsigned int i;
	float neighbourhood;

	/* calculate neighbourhood */
	if ( ( *som ).config.intParam[ NET_FUNCT ] == 0 )
		neighbourhood = gauss( dist, 1, 0, ( *som ).radius );
	else if ( ( *som ).config.intParam[ NET_FUNCT ] == 1 )
		neighbourhood = ricker( dist, ( *som ).radius );
	
	if ( neighbourhood < ( *som ).config.fltParam[ UPDATE_THRESH ] )
		neighbourhood = 0;

	/* update for element in vector */
	for ( i = 0; i < ( *som ).dim; i++ )
	{
		( *som ).weight[ x ][ y ][ i ] +=
			neighbourhood 
			* ( *som ).learnRate
			* ( ( *som ).input[ i ] 
			- ( *som ).weight[ x ][ y ][ i ] ); 
	}
	
}

/* find the closest distance, considering edge wrapping,
 * done by moving one vector to virtual positions */
float findWrapDist ( SelfOrganizingMap *som, float *v1, float *v2 )
{
	float dist, closest = FLT_MAX;
	int wrapMode = ( *som ).config.intParam[ NET_WRAP ];
	float w = ( *som ).config.intParam[ NET_WIDTH ];
	float h = ( *som ).config.intParam[ NET_HEIGHT ];

	/* regular distance, no wrapping */
	if ( wrapMode >= 0 && wrapMode <= 3 )
	{
		dist = euclidWrapper( 
			v1[ 0 ], v1[ 1 ], v2[ 0 ], v2[ 1 ] );
		if ( dist < closest )
			closest = dist;
	}
	/* horizontal wrapping I think */
	if ( wrapMode == 1 || wrapMode == 3 )
	{
		dist = euclidWrapper( 
			v1[ 0 ], v1[ 1 ], v2[ 0 ] - w, v2[ 1 ] );
		if ( dist < closest )
			closest = dist;
		
		dist = euclidWrapper( 
			v1[ 0 ], v1[ 1 ], v2[ 0 ] + w, v2[ 1 ] );
		if ( dist < closest )
			closest = dist;
	}
	/* vertical wrapping */
	if ( wrapMode == 2 || wrapMode == 3 )
	{
		dist = euclidWrapper( 
			v1[ 0 ], v1[ 1 ], v2[ 0 ], v2[ 1 ] - h );
		if ( dist < closest )
			closest = dist;

		dist = euclidWrapper( 
			v1[ 0 ], v1[ 1 ], v2[ 0 ], v2[ 1 ] + h );
		if ( dist < closest )
			closest = dist;
	}
	/* diagonal wrapping for when both wraps are used */
	if ( wrapMode == 3 )
	{
		dist = euclidWrapper( 
			v1[ 0 ], v1[ 1 ], v2[ 0 ] - w, v2[ 1 ] - h );
		if ( dist < closest )
			closest = dist;

		dist = euclidWrapper( 
			v1[ 0 ], v1[ 1 ], v2[ 0 ] - w, v2[ 1 ] + h );
		if ( dist < closest )
			closest = dist;
		
		dist = euclidWrapper( 
			v1[ 0 ], v1[ 1 ], v2[ 0 ] + w, v2[ 1 ] - h );
		if ( dist < closest )
			closest = dist;

		dist = euclidWrapper( 
			v1[ 0 ], v1[ 1 ], v2[ 0 ] + w, v2[ 1 ] + h );
		if ( dist < closest )
			closest = dist;

	}

	return closest;
}

/* four value wrapper for euclidian distance for above function */
float euclidWrapper ( float x1, float y1, float x2, float y2 )
{
	float v1[ 2 ];
	float v2[ 2 ];

	v1[ 0 ] = x1;
	v1[ 1 ] = y1;
	v2[ 0 ] = x2;
	v2[ 1 ] = y2;

	return euclid( v1, v2, 2 );
}

/* classify the data vectors */
void classifyData ( SelfOrganizingMap *som, DataSet *data, unsigned int *o )
{
	unsigned int x = ( *som ).config.intParam[ NET_WIDTH ];
	unsigned int y = ( *som ).config.intParam[ NET_HEIGHT ];
	unsigned int classified = 0;
	unsigned int i, j, k;

	int trainSize = ( 1.0 - ( *som ).config.fltParam[ HOLDOUT ] ) 
		* ( *data ).size;

	/* reset the match values */
	for ( j = 0; j < x; j++ )
		for ( k = 0; k < y; k++ )
			( *som ).match[ j ][ k ] = -1;

	float dist, bestDist;
	int closest[ 2 ];
	/* for every data vector, find node with closest weight that is not
	 * already assigned to another data vector */
	for ( i = 0; i < ( *data ).size; i++ )
	{
		bestDist = FLT_MAX;
		closest[ 0 ] = -1;
		closest[ 1 ] = -1;

		for ( j = 0; j < x; j++ )
		{
			for ( k = 0; k < y; k++ )
			{
				dist = euclid( ( *data ).vector[ o[ i ] ].value, 
					( *som ).weight[ j ][ k ],
					( *som ).dim );
				if ( ( dist < bestDist ) && 
					( ( *som ).match[ j ][ k ] == -1 ) )
				{
					bestDist = dist;
					closest[ 0 ] = j;
					closest[ 1 ] = k;
				}
			}
		}
		/* make association if within threshold limit */
		if ( bestDist <= ( *som ).config.fltParam[ MATCH_THRESH ] )
		{
			( *som ).match[ closest[ 0 ] ][ closest[ 1 ] ] = o[ i ];
			classified++;
		}
	}

	/* return total classification progress */
	( *som ).progress = classified;
}

/* classify areas of the lattice for classifying data vectors */
void classifyNodes ( SelfOrganizingMap *som, DataSet *data )
{
	unsigned int x = ( *som ).config.intParam[ NET_WIDTH ];
	unsigned int y = ( *som ).config.intParam[ NET_HEIGHT ];
	
	unsigned int closest;
	float dist, bestDist;
	
	unsigned int i, j, k;

	/* for every node in lattice, find closest data vector and take its
	 * classification for itself */
	for ( i = 0; i < x; i++ )
	{
		for ( j = 0; j < y; j++ )
		{
			bestDist = FLT_MAX;

			for ( k = 0; k < ( *data ).size; k++ )
			{
				if ( ( *data ).vector[ k ].testData )
					continue;

				dist = euclid( ( *data ).vector[ k ].value,
					( *som ).weight[ i ][ j ], 
					( *som ).dim );
				
				if ( dist < bestDist )
				{
					bestDist = dist;
					closest = k;
				}
			}
			
			/* if within threshold limit, classify the node */
			if ( bestDist <= 
				( *som ).config.fltParam[ CLASS_THRESH ] )
			{
				( *som ).class[ i ][ j ] = 
					( *data ).vector[ closest ].type;
			}
		}
	}
}

/* print weights for debug */
void printWeights ( SelfOrganizingMap *som )
{
	unsigned int i, j, k;

	for ( i = 0; i < ( *som ).config.intParam[ NET_WIDTH ]; i++ )
	{
		for ( j = 0; j < ( *som ).config.intParam[ NET_HEIGHT ]; j++ )
		{
			printf( "[%d, %d]:\n", i, j );
			for ( k = 0; k < ( *som ).dim; k++ )
				printf( "[%d] %4.2f ", k, 
					( *som ).weight[ i ][ j ][ k ] );
			printf( "\n\n" );
		}
	}
}

/* print match assignments for debug */
void printMatches ( SelfOrganizingMap *som )
{
	unsigned int i, j;

	printf( "Matches: \n" );
	for ( i = 0; i < ( *som ).config.intParam[ NET_WIDTH ]; i++ )
	{
		for ( j = 0; j < ( *som ).config.intParam[ NET_HEIGHT ]; j++ )
			if ( ( *som ).match[ i ][ j ] != -1 )
				printf( "%d  ", ( *som ).match[ i ][ j ] );
			else
				printf( "    " );

		printf( "\n\n" );
	}
}

/* print node classifications for debug */
void printClasses ( SelfOrganizingMap *som )
{
	unsigned int i, j;

	printf( "Classes: \n" );
	for ( i = 0; i < ( *som ).config.intParam[ NET_WIDTH ]; i++ )
	{
		for ( j = 0; j < ( *som ).config.intParam[ NET_HEIGHT ]; j++ )
			if ( ( *som ).class[ i ][ j ] != -1 )
				printf( "%d  ", ( *som ).class[ i ][ j ] );
			else
				printf( "    " );

		printf( "\n\n" );
	}
}

/* free the self organizing map from memory */
void freeSOM ( SelfOrganizingMap *som )
{
	unsigned int i, j;	

	if ( ( *som ).weight )
	{
		for ( i = 0; i < ( *som ).config.intParam[ NET_WIDTH ]; i++ )
		{
			for ( j = 0; j < ( *som ).config.intParam[ NET_HEIGHT ]; 
				j++ )
			{
				if ( ( *som ).weight[ i ][ j ] )
				{
					free( ( *som ).weight[ i ][ j ] );
					( *som ).weight[ i ][ j ] = NULL;
				}
			}
			
			if ( ( *som ).weight[ i ] )
			{
				free( ( *som ).weight[ i ] );
				( *som ).weight[ i ] = NULL;
			}
		}

		free( ( *som ).weight );
		( *som ).weight = NULL;
	}

	if ( ( *som ).match )
	{
		for ( i = 0; i < ( *som ).config.intParam[ NET_WIDTH ]; i++ )
		{
			if ( ( *som ).match[ i ] )
			{
				free( ( *som ).match[ i ] );
				( *som ).match[ i ] = NULL;
			}
		}

		free( ( *som ).match );
		( *som ).match = NULL;
	}
	
	if ( ( *som ).class )
	{
		for ( i = 0; i < ( *som ).config.intParam[ NET_WIDTH ]; i++ )
		{
			if ( ( *som ).class[ i ] )
			{
				free( ( *som ).class[ i ] );
				( *som ).class[ i ] = NULL;
			}
		}

		free( ( *som ).class );
		( *som ).class = NULL;
	}
}
