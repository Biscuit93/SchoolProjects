#include "defs.h"

extern float randomFloat (float min, float max);

int *createOrder (int size)
{
	int *order = malloc(size * sizeof(int));
	if (!order)
	{
		printf("Cannot allocate memory. Exiting...\n");
		exit(1);
	}
	
	for (int i = 0; i < size; i++)
		order[i] = i;
	
	return order;
}

void shuffleOrder (int *order, int size)
{		
	for (int i = 0; i < size; i++)
		order[i] = i;

	int j, temp;

	for (int i = size - 1; i > 0; i--)
	{
		j = (int) randomFloat(0, i);
		temp = order[i];
		order[i] = order[j];
		order[j] = temp;
	}	
}

void printOrder (int *order, int size)
{
	for (int i = 0; i < size; i++)
		printf("%8d%8d\n", i, order[i]);
}

void testOrder (int size)
{
	int *order = createOrder(size);
	shuffleOrder(order, size);
	printOrder(order, size);
	free(order);
}
