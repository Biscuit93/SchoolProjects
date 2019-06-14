#include "graphics.h"

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

char flash = 0, flashDelay = 0;

/* setup SDL graphics for visualization */
void initGraphics ()
{
	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "%s\n", SDL_GetError() );
		exit( 1 );
	}

	window = SDL_CreateWindow( PROGRAM_NAME, SDL_WINDOWPOS_UNDEFINED,
	        SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE );

	if ( !window )
	{
		printf( "%s\n", SDL_GetError() );
		exit( 1 );
	}

	renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
	if ( !renderer )
	{
		printf( "%s\n", SDL_GetError() );
		exit( 1 );
	}
	
	SDL_SetRenderDrawBlendMode( renderer, SDL_BLENDMODE_BLEND );

	SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
}

/* main draw method */
void draw ( SelfOrganizingMap *som, DataSet *data, unsigned int epoch )
{
	SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear( renderer );

	/* flash timer for mistake nodes */
	flashDelay++;
	if ( flashDelay >= 5 )
	{
		flashDelay = 0;
		flash--;
	}

	/* draw all the things */
	drawSOMStruct( som );
	drawSOMNodes( som, data );
	drawSOMProg( som, data, epoch );

	if ( epoch == ( *som ).config.intParam[ MAX_EPOCH ] )
		drawMistakes( som, data );

	SDL_RenderPresent( renderer );
}

/* draw the lattice */
void drawSOMStruct ( SelfOrganizingMap *som )
{
	SDL_SetRenderDrawColor( renderer, 0xC0, 0xC0, 0xC0, 0xFF );

	unsigned int wrapMode = ( *som ).config.intParam[ NET_WRAP ];

	unsigned int xWrap = 0;
	if ( ( wrapMode == 1 ) || ( wrapMode == 3 ) )
		xWrap = GFX_SPACING / 2;
	unsigned int yWrap = 0;
	if ( ( wrapMode == 2 ) || ( wrapMode == 3 ) )
		yWrap = GFX_SPACING / 2;

	unsigned int width = ( *som ).config.intParam[ NET_WIDTH ];
	unsigned int height = ( *som ).config.intParam[ NET_HEIGHT ];

	unsigned int i;
	for ( i = 0; i < width; i++ )
	{
		SDL_RenderDrawLine( renderer, 
			GFX_OFFSET + ( i * GFX_SPACING ), 
			GFX_OFFSET - yWrap,
			GFX_OFFSET + ( i * GFX_SPACING ), 
			GFX_OFFSET + ( ( height - 1 ) * GFX_SPACING ) + yWrap);
	}
	
	for ( i = 0; i < height; i++ )
	{
		SDL_RenderDrawLine( renderer, 
			GFX_OFFSET - xWrap, 
			GFX_OFFSET + ( i * GFX_SPACING ),
			GFX_OFFSET + ( ( width - 1 ) * GFX_SPACING ) + xWrap, 
			GFX_OFFSET + ( i * GFX_SPACING ) );
	}
}

/* draw the node and data classifications */
void drawSOMNodes ( SelfOrganizingMap *som, DataSet *data )
{
	unsigned int width = ( *som ).config.intParam[ NET_WIDTH ];
	unsigned int height = ( *som ).config.intParam[ NET_HEIGHT ];
	unsigned int match, class, testData;

	SDL_Rect nodeRect;

	unsigned int i, j;
	for ( i = 0; i < width; i++ )
	{
		for ( j = 0; j < height; j++ )
		{
			/* node classifications */
			if ( ( *som ).class[ i ][ j ] != -1 ) 
			{
				class = ( *som ).class[ i ][ j ];

				if ( class == 1 )
					SDL_SetRenderDrawColor( renderer, 
						0xFF, 0x00, 0x00, 0x80 );
				if ( class == 0 )
					SDL_SetRenderDrawColor( renderer, 
						0x00, 0x00, 0xFF, 0x80 );

				nodeRect.x = GFX_OFFSET 
					+ ( i * GFX_SPACING )
					- ( GFX_SPACING / 2 );
				nodeRect.y = GFX_OFFSET 
					+ ( j * GFX_SPACING )
					- ( GFX_SPACING / 2 );
				nodeRect.w = GFX_SPACING; 
				nodeRect.h = GFX_SPACING;

				SDL_RenderFillRect( renderer, &nodeRect );
			}
			
			/* data classifications */
			if ( ( *som ).match[ i ][ j ] != -1 )
			{
				match = ( *data ).vector[ 
					( *som ).match[ i ][ j ] ].type;
				testData = ( *data ).vector[ 
					( *som ).match[ i ][ j ] ].testData;

				if ( match == 1 )
					SDL_SetRenderDrawColor( renderer, 
						0xFF, 0x00, 0x00, 0xFF );
				if ( match == 0 )
					SDL_SetRenderDrawColor( renderer, 
						0x00, 0x00, 0xFF, 0xFF );
				nodeRect.x = GFX_OFFSET 
					+ ( i * GFX_SPACING )
					- ( GFX_NODESIZE / 2 );
				nodeRect.y = GFX_OFFSET 
					+ ( j * GFX_SPACING )
					- ( GFX_NODESIZE / 2 );
				nodeRect.w = GFX_NODESIZE; 
				nodeRect.h = GFX_NODESIZE;

				SDL_RenderFillRect( renderer, &nodeRect );

				/* data group classifications */
				if ( testData )
					SDL_SetRenderDrawColor( renderer, 
						0xFF, 0xFF, 0xFF, 0xFF );
				else
					SDL_SetRenderDrawColor( renderer, 
						0x00, 0x00, 0x00, 0xFF );

				nodeRect.x -= 1;
				nodeRect.y -= 1;
				nodeRect.w += 2; 
				nodeRect.h += 2;
				
				SDL_RenderDrawRect( renderer, 
					&nodeRect );
			}
		}
	}
}

/* draw the status bars */
void drawSOMProg ( SelfOrganizingMap *som, DataSet *data, unsigned int epoch )
{
	unsigned int width = ( ( *som ).config.intParam[ NET_WIDTH ] - 1 )
		* GFX_SPACING + 1;
	unsigned int offset = ( ( *som ).config.intParam[ NET_HEIGHT ]
		* GFX_SPACING ) + ( 2 * GFX_OFFSET );

	SDL_Rect barRect;

	barRect.x = GFX_OFFSET;
	barRect.y = offset;
	barRect.w = width;
	barRect.h = 4 * GFX_SPACING - 1;

	SDL_SetRenderDrawColor( renderer, 0xC0, 0xC0, 0xC0, 0xFF );
	SDL_RenderDrawRect( renderer, &barRect );

	barRect.x += 3;
	barRect.y += 3;
	barRect.h = GFX_SPACING - 4;
	
	/* epoch bar */
	barRect.w = calcBar( epoch, ( *som ).config.intParam[ MAX_EPOCH ], 
		width - 6 );
	SDL_RenderFillRect( renderer, &barRect );

	/* classification progress bar */
	barRect.y += GFX_SPACING - 1;
	barRect.w = calcBar( ( *som ).progress, ( *data ).size, width - 6 );
	SDL_RenderFillRect( renderer, &barRect );
	
	/* radius bar */
	barRect.y += GFX_SPACING - 1;
	barRect.w = calcBar( ( *som ).radius, 
		( *som ).config.fltParam[ RADIUS_INIT ], width - 6 );
	SDL_RenderFillRect( renderer, &barRect );
	
	/* learning rate bar */
	barRect.y += GFX_SPACING - 1;
	barRect.w = calcBar( ( *som ).learnRate, 
		( *som ).config.fltParam[ LEARN_INIT ], width - 6 );
	SDL_RenderFillRect( renderer, &barRect );
}

/* indicate misclassifications by flashing squares */
void drawMistakes ( SelfOrganizingMap *som, DataSet *data )
{
	unsigned int width = ( *som ).config.intParam[ NET_WIDTH ];
	unsigned int height = ( *som ).config.intParam[ NET_HEIGHT ];

	SDL_Rect nodeRect;

	unsigned int i, j;

	for ( i = 0; i < width; i++ )
	{
		for ( j = 0; j < height; j++ )
		{
			if ( ( ( *som ).match[ i ][ j ] != -1 ) &&
				( ( *data ).vector[ 
				( *som ).match[ i ][ j ] ].type
				!= ( *som ).class[ i ][ j ] ) )
			{
				SDL_SetRenderDrawColor( renderer, 
					0xFF, 0xFF, 0xFF, flash );
				nodeRect.x = GFX_OFFSET 
					+ ( i * GFX_SPACING )
					- ( GFX_SPACING / 2 );
				nodeRect.y = GFX_OFFSET 
					+ ( j * GFX_SPACING )
					- ( GFX_SPACING / 2 );
				nodeRect.w = GFX_SPACING; 
				nodeRect.h = GFX_SPACING;
				
				SDL_RenderFillRect( renderer, &nodeRect );
				
				SDL_SetRenderDrawColor( renderer, 
					0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderDrawRect( renderer, &nodeRect );
			}
		}
	}
}

/* calculate the progress bar sizes */
unsigned int calcBar ( float num, float denom, unsigned int max )
{
	return ( num / denom ) * max;
}

/* free the graphics library resources */
void cleanupGraphics ()
{
	SDL_DestroyRenderer( renderer );
	renderer = NULL;

	SDL_DestroyWindow( window );
	window = NULL;

	SDL_Quit();
}
