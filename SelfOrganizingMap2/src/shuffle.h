#include "defs.h"
#include "structs.h"

unsigned int *createOrder ( unsigned int size );
void shuffleOrder ( unsigned int **order, unsigned int size );
void printOrder ( unsigned int *order, unsigned int size );
void testOrder ( unsigned int size );

extern float randFlt ( float min, float max );
