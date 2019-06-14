#include "defs.h"

extern void new ();
extern void restart ();

extern unsigned int paused;

void getInput ()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				exit(0);
			break;

			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
						exit(0);
					break;
					case SDLK_n:
						new();
					break;
					case SDLK_p:
						if (paused)
							paused = 0;
						else
							paused = 1;
					break;
					case SDLK_q:
						exit(0);
					break;
					case SDLK_r:
						restart();
					break;
				}
		}
	}
}
