#include "defs.h"

extern void init ();
extern void cleanup ();
extern void draw ();
extern void getInput ();
extern void doEpoch (unsigned int t);

unsigned int timeStep;
unsigned int paused;

int main (int argc, char **argv)
{
	atexit(cleanup);
	init();

	while (1)
	{
		getInput();

		if (timeStep < MAX_EPOCH && !paused)
		{
			doEpoch(timeStep);
			timeStep++;
		}
		
		if (!DRAW_AFTER_EVERY_ADJUST)
		{	
			if ((timeStep - 1) % DRAW_EVERY_X_FRAME == 0)
				draw();

			if (FRAME_DELAY > 0)
				SDL_Delay(FRAME_DELAY);
		}
	}
	
	exit(0);
}
