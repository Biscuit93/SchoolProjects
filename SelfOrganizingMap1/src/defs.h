#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "SDL2/SDL.h"

#define PI acos(-1.0)

#define PROGRAM_NAME "Kohonen Map"

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600
#define FRAME_DELAY 0
#define DRAW_AFTER_EVERY_ADJUST 0
#define DRAW_EVERY_X_FRAME 1

#define SHOW_CONNECTIONS 1
#define SHOW_NEURONS 1
#define SHOW_NEURON_ZERO 0
#define NEURON_DRAW_SIZE 2
#define POINT_DRAW_SIZE 4

#define PROBLEM_SIZE 100
#define TSP_MAP_SIZE WINDOW_HEIGHT
#define TSP_MAP_BORDER_BUFFER 0.05
#define CHECK_ACTIVE_EVERY_X_EPOCH 1
#define THRESHOLD_DISTANCE 0.0001

#define VECTOR_DIMENSIONALITY 2
#define TARGET_DIMENSIONALITY 1

#define NETWORK_SIZE PROBLEM_SIZE * 3
#define MAX_EPOCH 1000000
#define INITIAL_RADIUS PROBLEM_SIZE / 2
#define MINIMUM_RADIUS 1.0
#define INITIAL_LEARNING_RATE 0.5
#define MINIMUM_LEARNING_RATE 0.1
#define LAMBDA 0.01
#define WRAP_NET 1
#define NOISE 0.0

typedef struct TSPMap
{
	unsigned int size, dim, width;
	float **point;
	int *active;
} 
TSPMap;

typedef struct Neuron
{
	unsigned int dim;
	float *weight;
	float output;
}
Neuron;

typedef struct Network
{	
	int wrap;
	unsigned int size, dim;
	float lambda;
	float initialRate, initialRadius;
	float rate, radius;
	float **input;
	Neuron *neuron;
}
Network;
