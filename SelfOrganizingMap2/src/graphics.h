#include "defs.h"
#include "structs.h"

void initGraphics ();
void draw ( SelfOrganizingMap *som, DataSet *data, unsigned int epoch );
void drawSOMStruct ( SelfOrganizingMap *som );
void drawSOMNodes ( SelfOrganizingMap *som, DataSet *data );
void drawSOMProg ( SelfOrganizingMap *som, DataSet *data, unsigned int epoch );
void drawMistakes ( SelfOrganizingMap *som, DataSet *data );
unsigned int calcBar ( float num, float denom, unsigned int max );
void cleanupGraphics ();

#define WINDOW_WIDTH 360
#define WINDOW_HEIGHT 500

#define GFX_OFFSET 40
#define GFX_SPACING 20
#define GFX_NODESIZE 10
