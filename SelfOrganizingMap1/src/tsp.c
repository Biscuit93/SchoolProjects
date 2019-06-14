#include "defs.h"

void restartTSP ();

extern float randomFloat (float min, float max);
extern float euclideanDistance (float *v1, float *v2, unsigned int n);
extern void new ();

TSPMap map;

extern Network net;

void initTSP ()
{	
	map.size = PROBLEM_SIZE;
	map.dim = VECTOR_DIMENSIONALITY;
	map.width = TSP_MAP_SIZE;
	
	map.point = malloc(map.size * sizeof(float*));
	if (!map.point)
	{
		printf("Could not allocate memory. Exiting...\n");
		exit(1);
	}

	for (int i = 0; i < map.size; i++)
	{
		map.point[i] = malloc (map.dim * sizeof(float));
		if (!map.point[i])
		{
			printf("Could not allocate memory. Exiting...\n");
			exit(1);
		}
	}

	map.active = malloc(map.size * sizeof(int));
	if (!map.active)
	{
		printf("Could not allocate memory. Exiting...\n");
		exit(1);
	}

	restartTSP();
}

void setActive (int x)
{
	if ((x != 0 && x != 1))
		return;

	for (int i = 0; i < map.size; i++)
		map.active[i] = x;
}

int checkIfActive ()
{
	int allActive = 1;

	for (int i = 0; i < map.size; i++)
	{
		map.active[i] = 0;
		for (int j = 0; j < net.size; j++)
		{
			if (euclideanDistance(map.point[i], 
				net.neuron[j].weight, VECTOR_DIMENSIONALITY)
				< THRESHOLD_DISTANCE)
			{
				map.active[i] = 1;
				break;
			}
		}

		if (map.active[i] == 0)
			allActive = 0;
	}
	
	return allActive;
}

void restartTSP ()
{
	for (int i = 0; i < map.size; i++)
	{
		map.active[i] = 0;

		for (int j = 0; j < map.dim; j++)
			map.point[i][j] = 
				randomFloat(0 + TSP_MAP_BORDER_BUFFER,
					1 - TSP_MAP_BORDER_BUFFER);
	}
}

void cleanupTSP ()
{
	for (int i = 0; i < map.size; i++)
	{
		if (map.point[i] != NULL)
		{
			free(map.point[i]);
			map.point[i] = NULL;
		}
	}

	if (map.point != NULL)
	{
		free(map.point);
		map.point = NULL;
	}

	if (map.active != NULL)
	{
		free(map.active);
		map.active = NULL;
	}
}
