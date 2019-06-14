#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <float.h>
#ifdef __linux__
	#include "SDL2/SDL.h"
#elif _WIN64
	#include <SDL.h>
#endif


#define AUTHOR "Anthony Edward Ferski"
#define PROGRAM_NAME "4P80A2"

#define CANT_ALLOC "Cannot allocate required memory. Exiting..."
#define CANT_OPEN "cannot be opened."

#define CONFIG "data/params.cfg"
#define FILE1 "data/L30fft16.out"
#define FILE2 "data/L30fft25.out"
#define FILE3 "data/L30fft32.out"
#define FILE4 "data/L30fft64.out"

enum int_parameters
{
	SEED, DATASET, NET_WIDTH, NET_HEIGHT, NET_WRAP, NET_FUNCT, 
	NET_CUTOFF, MAX_EPOCH, 
	INTPARAM_COUNT
};

enum flt_parameters
{
	RADIUS_INIT, RADIUS_DECAY, RADIUS_MIN, LEARN_INIT, LEARN_DECAY, 
	LEARN_MIN, HOLDOUT, UPDATE_THRESH, MATCH_THRESH, CLASS_THRESH,
	FLTPARAM_COUNT
};
