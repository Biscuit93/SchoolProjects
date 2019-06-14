#include "defs.h"

extern void initGraphics ();
extern void cleanupGraphics ();

extern void initTSP ();
extern void restartTSP ();
extern void cleanupTSP ();

extern void initSOM ();
extern void restartSOM ();
extern void cleanupSOM ();

extern unsigned int timeStep;
extern unsigned int paused;

void init ()
{
	time_t currentTime = time(NULL);
	srand(currentTime);

	timeStep = 1;
	paused = 0;

	initGraphics();
	initTSP();
	initSOM();
}

void new ()
{
	timeStep = 1;
	paused = 0;

	restartTSP();
	restartSOM();
}

void restart ()
{
	timeStep = 1;
	paused = 0;

	restartSOM();
}

void cleanup ()
{
	cleanupGraphics();
	cleanupTSP();
	cleanupSOM();
}
