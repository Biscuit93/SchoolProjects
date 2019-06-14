#include "defs.h"

int findBMU ();
void setParameters ();
void setWeights ();
void setWeightsToCities ();
void setWeightsToRandom ();
void updateWeights (int winner, unsigned int t);

extern void draw ();
extern float randomFloat (float min, float max);
extern float euclideanDistance (float *v1, float *v2, unsigned int n);
extern float exponentialDecay (float time, float initial, float lambda);
extern float gaussCurve (float x, float a, float b, float c);
extern float mexicanHatWave (float time, float radius);
extern int compareInts (int x, int y, int larger);
extern float compareFloats (float x, float y, int larger);
extern float distanceFromBMU (float time, float radius, float distance);
extern void setActive (int x);
extern int checkIfActive ();
extern int *createOrder (int size);
extern int *shuffleOrder (int *order, int size);

Network net;

extern TSPMap map;

void initSOM ()
{
	setParameters();
	
	net.neuron = malloc(net.size * sizeof(Neuron));
	if (!net.neuron)
	{
		printf("Could not allocate memory. Exiting...\n");
		exit(1);
	}

	for (int i = 0; i < net.size; i++)
	{
		net.neuron[i].dim = VECTOR_DIMENSIONALITY;

		net.neuron[i].weight = malloc(net.neuron[i].dim * 
			sizeof(float));
		if (!net.neuron[i].weight)
		{
			printf("Could not allocate memory. Exiting...\n");
			exit(1);
		}
	}

	setWeights();
}

void doEpoch (unsigned int t)
{
	int *inputOrder = NULL;
	inputOrder = createOrder(PROBLEM_SIZE);
	shuffleOrder(inputOrder, PROBLEM_SIZE);
	int winner = 0;

	for (int i = 0; i < PROBLEM_SIZE; i++)
	{
		net.input = &(map.point[i]);
		net.radius = compareFloats(exponentialDecay(t, 
			net.initialRadius, net.lambda), MINIMUM_RADIUS, 1);
		net.rate = compareFloats(exponentialDecay(t, net.initialRate, 
			net.lambda), MINIMUM_LEARNING_RATE, 1);
		
		winner = findBMU();
		updateWeights(winner, t);

		if (DRAW_AFTER_EVERY_ADJUST)
		{
			draw();

			if (FRAME_DELAY > 0)
				SDL_Delay(FRAME_DELAY);
		}
	}

	if (inputOrder != NULL)
	{
		free(inputOrder);
		inputOrder = NULL;
	}
	if ((t % CHECK_ACTIVE_EVERY_X_EPOCH) == 0)
		checkIfActive();
}

int findBMU ()
{
	int best = 0;
	float distance = 0, lowest = 1;

	for (int i = 0; i < net.size; i++)
	{
		distance = euclideanDistance(*(net.input), 
		                             net.neuron[i].weight,
					     VECTOR_DIMENSIONALITY);
		if (distance < lowest)
		{
			lowest = distance;
			best = i;
		}
	}

	return best;
}

void updateWeights (int best, unsigned int t)
{
	int distance; 

	for (int i = 0; i < net.size; i++)
	{
		if (net.wrap)
			distance = compareInts(abs(best - i), 
				abs(best - (i - net.size)), 0);
		else
			distance = abs(best - i);

		if (distance <= net.radius)
			for (int j = 0; j < 2; j++)
				net.neuron[i].weight[j] +=
					distanceFromBMU(t, 
						net.radius ,distance) *
					net.rate * ((*net.input)[j] - 
					net.neuron[i].weight[j] -
					randomFloat(-(NOISE / 2), NOISE / 2));
	}
}

void restartSOM ()
{
	setParameters();
	setActive(0);
	setWeights();
}

void setParameters ()
{
	net.wrap = WRAP_NET;
	net.size = NETWORK_SIZE;
	net.dim = TARGET_DIMENSIONALITY;
	net.lambda = LAMBDA;
	net.initialRate = INITIAL_LEARNING_RATE;
	net.initialRadius = INITIAL_RADIUS;
	net.input = NULL;
}

void setWeights ()
{
	if (PROBLEM_SIZE == NETWORK_SIZE)
		setWeightsToCities();
	else
		setWeightsToRandom();
}

void setWeightsToRandom ()
{
	for (int i = 0; i < net.size; i++)
		for (int j = 0; j < net.neuron[i].dim; j++)
			net.neuron[i].weight[j] = randomFloat(0, 1);
}

void setWeightsToCities ()
{
	int *initOrder = NULL;
	initOrder = createOrder(net.size);
	shuffleOrder(initOrder, net.size);

	for (int i = 0; i < net.size; i++)
		for (int j = 0; j < net.neuron[i].dim; j++)
			net.neuron[i].weight[j] = map.point[initOrder[i]][j];

	if (initOrder != NULL)
	{
		free(initOrder);
		initOrder = NULL;
	}
}

void cleanupSOM ()
{
	if (net.neuron != NULL)
	{
		for (int i = 0; i < net.size; i++)
		{
			if (net.neuron[i].weight != NULL)
			{
				free(net.neuron[i].weight);
				net.neuron[i].weight = NULL;
			}
		}

		free(net.neuron);
		net.neuron = NULL;
	}
}
