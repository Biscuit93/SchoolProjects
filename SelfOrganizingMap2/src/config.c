#include "config.h"

/* load a configuration file from disk or set to default */
Config loadConfig ( unsigned int cID )
{
	Config conf;
	conf.configID = cID;

	/* if file not found, load default */ 
	FILE *file = fopen( CONFIG, "r" );
	if ( !file )
	{
		printf( "%s %s\n", CONFIG, CANT_OPEN );
		printf( "Using default parameters.\n" );
		defConfig( &conf );
		return conf;
	}
	
	unsigned int i;

	/* load the parameters */
	conf.intParam[ SEED ] = time( NULL );
	for ( i = 1; i < INTPARAM_COUNT; i++ )
		fscanf( file, "%d", &conf.intParam[ i ] );
	for ( i = 0; i < FLTPARAM_COUNT; i++ )
		fscanf( file, "%f", &conf.fltParam[ i ] );

	fclose( file );
	return conf;
}

/* set the configuration to these defaults */
void defConfig ( Config *conf )
{
	( *conf ).intParam[ SEED ] = time( NULL );
	( *conf ).intParam[ DATASET ] = 0;
	( *conf ).intParam[ NET_WIDTH ] = 15;
	( *conf ).intParam[ NET_HEIGHT ] = 15;
	( *conf ).intParam[ NET_WRAP ] = 3;
	( *conf ).intParam[ NET_FUNCT ] = 0;
	( *conf ).intParam[ NET_CUTOFF ] = 0;
	( *conf ).intParam[ MAX_EPOCH ] = 1000;

	( *conf ).fltParam[ RADIUS_INIT ] = 7.5;
	( *conf ).fltParam[ RADIUS_DECAY ] = 0.005;
	( *conf ).fltParam[ RADIUS_MIN ] = 0.0;
	( *conf ).fltParam[ LEARN_INIT ] = 1.0;
	( *conf ).fltParam[ LEARN_DECAY ] = 0.005;
	( *conf ).fltParam[ LEARN_MIN ] = 0.0;
	( *conf ).fltParam[ HOLDOUT ] = 0.15;
	( *conf ).fltParam[ UPDATE_THRESH ] = 0.01;
	( *conf ).fltParam[ MATCH_THRESH ] = 1000.0;
	( *conf ).fltParam[ CLASS_THRESH ] = 1000.0;
}

/* print out for debug */
void printConfig ( Config *conf )
{
	printf( "ID %d\n", ( *conf ).configID );

	unsigned int i;

	for ( i = 0; i < INTPARAM_COUNT; i++ )
		printf( "INT %d: %d\n", i, ( *conf ).intParam[ i ] );

	for ( i = 0; i < FLTPARAM_COUNT; i++ )
		printf( "FLT %d: %f\n", i, ( *conf ).fltParam[ i ] );
}
