#include "defs.h"
#include "structs.h"

SelfOrganizingMap loadSOM ( Config conf, DataSet data );
void initWeights ( SelfOrganizingMap *som, DataSet *data );
void trainSOM ( SelfOrganizingMap *som, DataSet *data );
void findBMU ( SelfOrganizingMap *som, unsigned int *x, unsigned int *y );
void updateWeights ( SelfOrganizingMap *som, unsigned int x, unsigned int y,
	unsigned int t );
void updateVector ( SelfOrganizingMap *som, unsigned int x, unsigned int y,
	float dist );
float findWrapDist ( SelfOrganizingMap *som, float *v1, float *v2 );
float euclidWrapper ( float x1, float y1, float x2, float y2 );
void classifyData ( SelfOrganizingMap *som, DataSet *data, unsigned int *o );
void classifyNodes ( SelfOrganizingMap *som, DataSet *data );
void printWeights ( SelfOrganizingMap *som );
void printMatches ( SelfOrganizingMap *som );
void printClasses ( SelfOrganizingMap *som );
void freeSOM ( SelfOrganizingMap *som );

extern float randFlt ( float min, float max );
extern unsigned int *createOrder ( unsigned int size );
extern void shuffleOrder ( unsigned int **order, unsigned int size );
extern float gauss ( float x, float a, float b, float c );
extern float ricker ( float x, float a );
extern float euclid ( float *v1, float *v2, unsigned int n );
extern float decay ( float x, float i, float d );
extern float compFlts ( float x, float y, int larger );
extern void draw ( SelfOrganizingMap *som, DataSet *data, unsigned int epoch );
extern void getInput ();
void printOrder ( unsigned int *order, unsigned int size );
