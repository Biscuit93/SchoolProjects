#include "init.h"

extern DataSet d;
extern Config c;
extern SelfOrganizingMap som;

/* at program start do this */
void init ()
{
	atexit( cleanup );
	initGraphics();
	start();
}

/* get the show on the road  */
void start ()
{
	c = loadConfig( 1 );
	d = loadData( c.intParam[ DATASET ] );
	som = loadSOM( c, d );
	
	trainSOM( &som, &d );
}

/* restarting the main event */
void restart ()
{
	freeSOM( &som );
	freeData( &d );	
	start();
}

/* pack it all up at exit */
static void cleanup ()
{
	freeSOM( &som );
	freeData( &d );

	cleanupGraphics();
}
