#include "defs.h"

float randomFloat (float min, float max)
{
	if (min >= max)
		return 0;

	return ((float) rand() / RAND_MAX) * (max - min) + min;
}

float euclideanDistance (float *v1, float *v2, unsigned int n)
{
	float distance = 0;

	for (int i = 0; i < n; i++)
		distance += (v1[i] - v2[i]) * (v1[i] - v2[i]);

	return sqrt(distance);
}

float exponentialDecay (float time, float initial, float lambda)
{
	return (initial * exp(-(lambda * time)));
}

float gaussCurve (float x, float a, float b, float c)
{
	return (a * exp(-(pow((x - b), 2) / (2 * pow(c, 2)))));
}

float mexicanHatWave (float time, float radius)
{
	return ((2 / (sqrt(3 * radius) * pow(PI, 0.25)) * 
		(1 - ((time * time) / (radius * radius))) * 
		(exp((-(time * time)) / (2 * (radius * radius))))));
}

float distanceFromBMU (float time, float radius, float distance)
{
	return (exp((-(distance * distance) / (2 * (radius * radius) * time))));
}

int compareInts (int x, int y, int larger)
{
	if (larger)
	{
		if (x >= y)
			return x;
		else 
			return y;
	}
	else
	{
		if (x <= y)
			return x;
		else
			return y;
	}
}

int compareFloats (float x, float y, int larger)
{
	if (larger)
	{
		if (x >= y)
			return x;
		else 
			return y;
	}
	else
	{
		if (x <= y)
			return x;
		else
			return y;
	}
}

void testMath (float upper, float step)
{
	float decay = 0;
	float gauss = 0;
	
	for (float f = 0; f <= upper; f += step)
	{
		decay = exponentialDecay(f, 1, 1);
		gauss = gaussCurve(f, 1, 0, 1);

		printf("%8.8f\t%8.8f\t%8.8f\t%8.8f\n", 
			f, decay, gauss, mexicanHatWave(f, 1));
	}
}
