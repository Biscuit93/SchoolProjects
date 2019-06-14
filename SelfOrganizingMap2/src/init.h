#include "defs.h"
#include "structs.h"

void init ();
void start ();
void restart ();
static void cleanup ();

extern void initGraphics ();
extern void cleanupGraphics ();

extern SelfOrganizingMap loadSOM ( Config conf, DataSet data );
extern DataSet loadData ( unsigned int dataFile );
extern Config loadConfig ( unsigned int cID );

extern void trainSOM ( SelfOrganizingMap *som, DataSet *data );

extern void freeSOM ( SelfOrganizingMap *som );
extern void freeData ( DataSet *data );
