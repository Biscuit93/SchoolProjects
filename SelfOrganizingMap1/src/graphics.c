#include "defs.h"

void drawTSPMap ();
void drawSOM ();
void drawCross (int x, int y, unsigned int size);
void drawSquare (int x, int y, unsigned int size);

extern float euclideanDistance (float *v1, float *v2);

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

extern TSPMap map;
extern Network net;

void initGraphics ()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("%s\n", SDL_GetError());
		exit(1);
	}

	window = SDL_CreateWindow(PROGRAM_NAME, SDL_WINDOWPOS_UNDEFINED,
	                          SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH,
				  WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if (!window)
	{
		printf("%s\n", SDL_GetError());
		exit(1);
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer)
	{
		printf("%s\n", SDL_GetError());
		exit(1);
	}

	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
}

void testEuclid ()
{
	SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
	SDL_RenderDrawLine(renderer, 
			   map.point[1][0] * map.width,
	                   map.point[1][1] * map.width,
			   net.neuron[1].weight[0] * map.width,
			   net.neuron[1].weight[1] * map.width);

	float dist = euclideanDistance((map.point[1]), (net.neuron[1].weight));
	dist *= map.width;

	printf("Distance: %1.4f\n", dist);
}

void draw ()
{
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);

	drawSOM();
	drawTSPMap();
	
	SDL_RenderPresent(renderer);
}

void drawSOM ()
{
	unsigned int pointWidth = NEURON_DRAW_SIZE;

	int x1, x2, y1, y2;

	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
	for (int i = 0; i < net.size; i++)
	{
		if (SHOW_NEURONS)
			drawCross(net.neuron[i].weight[0] * map.width,
				  net.neuron[i].weight[1] * map.width, 
				  pointWidth);

		if ((i == 0) && SHOW_NEURON_ZERO)
			drawSquare(net.neuron[i].weight[0] * map.width,
				net.neuron[i].weight[1] * map.width, 
				pointWidth);

		if ((net.wrap || i < net.size - 1) && SHOW_CONNECTIONS)
		{
			x1 = net.neuron[i].weight[0] * map.width;
			y1 = net.neuron[i].weight[1] * map.width;
			x2 = net.neuron[(i + 1) % net.size].weight[0] *
			     map.width;
			y2 = net.neuron[(i + 1) % net.size].weight[1] *
			     map.width;
			SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
		}
	}

}

void drawTSPMap ()
{
	unsigned int squareWidth = POINT_DRAW_SIZE;

	SDL_Rect mapOutline = {0, 0, map.width, map.width};
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderDrawRect(renderer, &mapOutline);

	for (int i = 0; i < map.size; i++)
	{
		if (map.active[i])
			SDL_SetRenderDrawColor(renderer, 
				0x00, 0xFF, 0x00, 0xFF);
		else
			SDL_SetRenderDrawColor(renderer, 
				0xFF, 0x00, 0x00, 0xFF);

		drawSquare(map.point[i][0] * map.width, 
		           map.point[i][1] * map.width, squareWidth);
	}
}

void drawCross (int x, int y, unsigned int size)
{
	SDL_RenderDrawLine(renderer, x - (size / 2), y, x + (size / 2), y);
	SDL_RenderDrawLine(renderer, x, y - (size / 2), x, y + (size / 2));
}

void drawSquare (int x, int y, unsigned int size)
{
	SDL_RenderDrawLine(renderer, x - (size / 2), y - (size / 2),
	                   x + (size / 2), y - (size / 2));
	SDL_RenderDrawLine(renderer, x - (size / 2), y + (size / 2),
	                   x + (size / 2), y + (size / 2));
	SDL_RenderDrawLine(renderer, x - (size / 2), y - (size / 2),
	                   x - (size / 2), y + (size / 2));
	SDL_RenderDrawLine(renderer, x + (size / 2), y - (size / 2),
	                   x + (size / 2), y + (size / 2));
}

void cleanupGraphics ()
{
	SDL_DestroyRenderer(renderer);
	renderer = NULL;

	SDL_DestroyWindow(window);
	window = NULL;

	SDL_Quit();
}
