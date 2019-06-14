/* Experimental Feed-Forward Neural Network
 * Written by Anthony Ferski for COSC4P80 Assignment 1
 * 5192885 / AF11MB
 * 
 * not entirely complete to assignment specifications.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define SYMBOL_TYPES "0123456789"
#define TYPE_COUNT 10
#define SYMBOL_LENGTH 64
#define SYMBOL_WIDTH 8
#define TRAIN_DATA_PATH "data/train/digit_train_x.txt"
#define TRAIN_PATH_OFFSET 23
#define TEST_DATA_PATH "data/test/digit_test_x.txt"
#define TEST_PATH_OFFSET 21

/* a single digit */
typedef struct Symbol
{
	char type;
	int length, width;
	float *data;
} Symbol;

/* a set of digits */
typedef struct SymbolSet
{
	int size;
	Symbol *symbol;
} SymbolSet;

/* a neuron */
typedef struct Neuron
{
	float input, output, error;
	float (*forward)(float), (*backward)(float);
} Neuron;

/* a neural network */
typedef struct NeuralNetwork
{
	int layers, nodes;
	int *structure;
	Neuron **neuron;
	float ***connection;
} NeuralNetwork;

/* functions */
void printTitle ();
void setSeed ();
void loadAllSymbols ();
void loadCombinedSymbolSet (SymbolSet *destination, char *filePath, int offset);
SymbolSet createSymbolSet (char *filePath, char type, int length, int width);
Symbol createSymbol (char type, int length, int width, char *data);
void printSymbol (Symbol symbol);
void printSymbolSet (SymbolSet set);
void initializeNetwork (int layers, int nodes, int structure[]);
float initialWeight ();
void printNetworkWeights (NeuralNetwork net);
void trainNetwork ();
void testNetwork ();
float performTrainingEpoch ();
float logistic (float x);
float hyperbolic (float x);
float logisticDerivative (float x);
float hyperbolicDerivative (float x);
char forwardPass (Symbol s);
void backwardProp (char expected);
void testAllFiles ();
int *createOrder (int size);
void shuffleNumbers (int *list, int size);
int countLinesInFile (FILE *file);
int findLongestLineLength (FILE *file);
void cleanup ();
void freeAllSymbols ();
void freeSymbolSet (SymbolSet set);
void freeNetwork (NeuralNetwork net);

/* globals */
SymbolSet trainingSet;
SymbolSet testingSet;
NeuralNetwork network;

float learningRate;
float trainingCutoff;
float epochLimit;
char activationFunction;





/* the main function */
int main (int argc, char **argv)
{
	atexit(cleanup);
	
	printTitle();
	setSeed();
	loadAllSymbols();
	
	/* settings here */
	learningRate = 0.05;
	trainingCutoff = 0.99;
	epochLimit = 10;
	/* note: L = logistic, H = hyperbolic */
	activationFunction = 'L';
	
	/* set up network */
	int structure[3] = {64, 32, 10};
	/* note: 2nd param = total of above */
	initializeNetwork(3, 106, structure);
		
	/* train and test */
	trainNetwork();
	testNetwork();
		
	/* done */	
	exit(0);
}

/* information about the program and author */
void printTitle ()
{
	printf("Experimental Feed-Forward Neural Network\n");
	printf("Written by Anthony Ferski for COSC4P80 Assignment 1\n");
	printf("5192885 / AF11MB\n\n");
}

/* set the RNG seed to current UNIX time */
void setSeed ()
{
	time_t currentTime = time(NULL);
	srand(currentTime);
}

/* load all the digits into memory */
void loadAllSymbols ()
{
	printf("Loading all symbols...\n\n");
	
	printf("\tLoading training set...\n");
	loadCombinedSymbolSet(&trainingSet, TRAIN_DATA_PATH, TRAIN_PATH_OFFSET);
	printf("\tLoading testing set...\n");
	loadCombinedSymbolSet(&testingSet, TEST_DATA_PATH, TEST_PATH_OFFSET);
	
	printf("All symbols loaded.\n\n");
}

/* load seperate files into a single set */
void loadCombinedSymbolSet (SymbolSet *destination, char *filePath, int offset)
{
	FILE *file;
	
	/* variable filepath to change 10 times */
	char *variablePath = malloc(30 * sizeof(char));
	int totalSymbols = 0, subsetSize = 0;
	
	/* count how many digits in total */
	strcpy(variablePath, filePath);
	for (int i = 0; i < TYPE_COUNT; i++) 
	{
		variablePath[offset] = SYMBOL_TYPES[i];
		file = fopen(variablePath, "r");
		totalSymbols += countLinesInFile(file);
		/* how many of each digit are there */
		if (i == 0)
			subsetSize = totalSymbols;
		fclose(file);
	}
	
	/* allocate memory for all symbols */
	(*destination).size = totalSymbols;
	(*destination).symbol = malloc((*destination).size * sizeof(Symbol));
	SymbolSet temp;

	/* load each digit file seperately and combine them */
	for (int i = 0; i < TYPE_COUNT; i++)
	{
		variablePath[offset] = SYMBOL_TYPES[i];
		temp = createSymbolSet(variablePath, SYMBOL_TYPES[i],
		                       SYMBOL_LENGTH, SYMBOL_WIDTH);
		                       
		for (int s = 0; s < subsetSize; s++)
			(*destination).symbol[s + (i * subsetSize)] 
				= temp.symbol[s];
	}	
	
	printf("\t%d symbols loaded.\n\n", totalSymbols);
}

/* create a symbolset for each digit to be combined later */
SymbolSet createSymbolSet (char *filePath, char type, int length, int width)
{
	SymbolSet set;
	
	/* open file and confirm */
	FILE *file = fopen(filePath, "r");
	
	if (!file)
	{
		printf("Cannot open file %s. Aborting...\n", filePath);
		exit(1);
	}
	
	/* allocate memory for all digits and confirm */
	set.size = countLinesInFile(file);
	set.symbol = malloc(set.size * sizeof(Symbol));
	
	if (!set.symbol)
	{
		printf("Cannot allocate memory. Aborting...\n");
		exit(1);
	}
	
	/* allocate memory to hold each data string in file temporarily */
	int lineLength = findLongestLineLength(file) + 10;
	char *data = malloc(lineLength * sizeof(char));
	
	if (!data)
	{
		printf("Cannot allocate memory. Aborting...\n");
		exit(1);
	}
	
	char *ignore;

	/* create a symbol for each digit data string */
	for (int i = 0; i < set.size; i++)
	{
		ignore = fgets(data, lineLength, file);
		set.symbol[i] = createSymbol(type, length, width, data);
	}

	if (0)
		printf("%s\n", ignore);
	
	printf("\t\tLoaded %d symbols from %s\n", set.size, filePath);
	
	/* free temp memory and return */
	free(data);
	return set;
}

/* creates a symbol from a digit data string from the files */
Symbol createSymbol (char type, int length, int width, char *data)
{
	Symbol symbol;
	
	/* set all the symbol variables from params */
	symbol.type = type;
	symbol.length = length;
	symbol.width = width;
	symbol.data = malloc(symbol.length * sizeof(float));
	
	/* tokenize the data string */
	char *token = strtok(data, " ,\n");
	
	/* convert each token to float and load to memory */
	for (int i = 0; i < symbol.length; i++)
	{
		symbol.data[i] = atof(token);
		token = strtok(NULL, " ,\n");
	}
		
	return symbol;
}

/* prints an ascii representation of a symbol */
void printSymbol (Symbol symbol)
{
	float data;
	
	for (int i = 0; i < symbol.length; i++)
	{
		data = symbol.data[i];
		
		/* print a 'pixel' */
		if (data > 0.8)
			printf("#");
		else if (data > 0.6)
			printf("=");
		else if (data > 0.4)
			printf(":");
		else if (data > 0.2)
			printf("-");
		else
			printf(" ");
			
		if (((i + 1) % symbol.width) == 0)
			printf("\n");
	}
}

/* same as above but for all symbols in a set and multiple symbols in a row */
void printSymbolSet (SymbolSet set)
{
	/* complicated logic to get symbols to print side-by-side */
	int symbolsPerRow = 8;
	int currentRow = 0, currentSymbol = 0, currentDatum = 0;
	int lineLength = symbolsPerRow * SYMBOL_WIDTH, 
	    rowLength = symbolsPerRow * SYMBOL_LENGTH;
	float data = 0;
	for (int x = 0; x < set.size * SYMBOL_LENGTH; x++)
	{
		/* i seriously forgot how this works */
		currentRow = x / rowLength;
		currentSymbol = (currentRow * symbolsPerRow) + 
		                ((x / SYMBOL_WIDTH) % symbolsPerRow);
		currentDatum = ((x % rowLength) % SYMBOL_WIDTH) +
		               (((x % rowLength) / lineLength) * SYMBOL_WIDTH);
		
		data = set.symbol[currentSymbol].data[currentDatum];
		
		/* print the digit type */
		if ((x % rowLength) == 0)
		{
			printf("\n");
			
			for (int i = 0; i < symbolsPerRow; i++)
			{
				printf("%c", set.symbol[currentSymbol].type);
				for (int j = 0; j < SYMBOL_WIDTH; j++)
					printf(" ");
			}
			
			printf("\n");
		}
		
		/* print a 'pixel' */
		if (data > 0.8)
			printf("#");
		else if (data > 0.6)
			printf("=");
		else if (data > 0.4)
			printf(":");
		else if (data > 0.2)
			printf("-");
		else
			printf(" ");
		
		/* a space between digits */
		if (((x + 1) % SYMBOL_WIDTH) == 0)
			printf(" ");
		
		/* a new line between rows of digits */
		if (((x + 1) % lineLength) == 0)
			printf("\n");
	}
	
	printf("\n");
}

/* setup the neural network */
void initializeNetwork (int layers, int nodes, int structure[])
{
	/* total neurons in net */
	int nodeCount = 0;
	
	for (int l = 0; l < layers; l++) 
		nodeCount += structure[l];
		
	/* confirm that the numbers make sense */
	if (nodeCount != nodes)
	{
		printf("Invalid Configuration. Aborting...\n");
		exit(1);
	}
	
	/* set all the network variables from params */
	network.layers = layers;
	network.nodes = nodes;
	
	/* tons of memory allocation incomming */
	network.structure = malloc(layers * sizeof(int));
	
	if (!network.structure)
	{
		printf("Cannot allocate memory. Aborting...\n");
		exit(1);
	}
	
	for (int l = 0; l < layers; l++)
		network.structure[l] = structure[l];

	network.neuron = malloc(layers * sizeof(Neuron*));
	
	if (!network.neuron)
	{
		printf("Cannot allocate memory. Aborting...\n");
		exit(1);
	}
	
	for (int l = 0; l < network.layers; l++)
	{
		network.neuron[l] = malloc(network.structure[l] * 
		                    sizeof(Neuron));
		                    
		if (!network.neuron[l])
		{
			printf("Cannot allocate memory. Aborting...\n");
			exit(1);
		}
		
		for (int n = 0; n < network.structure[l]; n++)
		{
			/* set each neuron's params and activation functions */
			network.neuron[l][n].input = 0;
			network.neuron[l][n].output = 0;
			network.neuron[l][n].error = 0;
			
			if (activationFunction == 'L')
			{
				network.neuron[l][n].forward = 
					&logistic;
				network.neuron[l][n].backward = 
					&logisticDerivative;
			}
			else if (activationFunction == 'H')
			{
				network.neuron[l][n].forward = 
					&hyperbolic;
				network.neuron[l][n].backward = 
					&hyperbolicDerivative;
			}
			else
			{
				printf("Invalid Configuration. Aborting...\n");
				exit(1);
			}
		}
	}
	
	/* more mallocs, these are for the network weights */
	network.connection = malloc((network.layers - 1) * sizeof(float**));
	
	if (!network.connection)
	{
		printf("Cannot allocate memory. Aborting...\n");
		exit(1);
	}
	
	for (int l = 0; l < network.layers - 1; l++)
	{
		network.connection[l] = malloc(network.structure[l] * 
		                               sizeof(float*));
		
		if (!network.connection[l])
		{
			printf("Cannot allocate memory. Aborting...\n");
			exit(1);
		}
		
		for (int s = 0; s < network.structure[l]; s++)
		{
			network.connection[l][s]
				= malloc(network.structure[l + 1] * 
				         sizeof(float));
				         
			if (!network.connection[l][s])
			{
				printf("Cannot allocate memory. Aborting...\n");
				exit(1);
			}
		
			/* initialize each weight */
			for (int r = 0; r < network.structure[l + 1]; r++)
				network.connection[l][s][r] = initialWeight();

		}
	}	
}

/* return a random number between -0.5 and 0.5 */
float initialWeight ()
{
	return (((float) rand()) / RAND_MAX) - 0.5;
}

/* print all the weights as integers x100 */
void printNetworkWeights (NeuralNetwork net)
{
	for (int l = 0; l < net.layers - 1; l++)
	{
		printf("Layer %d:\n", l);
		for (int s = 0; s < net.structure[l]; s++)
		{
			printf("Neuron %d:\n", s);
			for (int r = 0; r < net.structure[l + 1]; r++)
			{
				printf("%3.0f ", net.connection[l][s][r] * 100);
			}
			printf("\n");
		}
		printf("\n");
	}
}

/* activation functions */

float logistic (float x)
{
	return (1 / (1 + exp(-x)));
}

float hyperbolic (float x)
{
	return tanh(x);
}

float logisticDerivative (float x)
{
	return (logistic(x) * (1 - logistic(x)));
}

float hyperbolicDerivative (float x)
{
	return (1 - (pow(tanh(x), 2)));
}

/* network training procedure */
void trainNetwork ()
{	
	time_t trainingStartTime = time(NULL);
	
	printf("Training neural network...\n\n");
	
	/* stores the last 80 accuracy values */
	float accuracyGraph[80];
	
	for (int i = 0; i < 80; i++)
	{
		accuracyGraph[i] = 0;
	}
	
	int epoch = 0;
	float accuracy = 0;
	
	/* train while not accurate enough or within epoch limit */
	while (accuracy < trainingCutoff && epoch < epochLimit)
	{
		epoch++;
		printf("\tEpoch %d:\n", epoch);
		accuracy = performTrainingEpoch();
		
		for (int i = 80 - 1; i > 0; i--)
			accuracyGraph[i] = accuracyGraph[i - 1];
			
		accuracyGraph[0] = accuracy;
	}
	
	/* record time elapsed */
	time_t trainingDuration = time(NULL) - trainingStartTime;
	
	float lowest = 1, highest = 0;
	
	/* find lowest and highest accuracy from last 80 for ascii graph */
	for (int i = 0; i < 80; i++)
	{
		if (accuracyGraph[i] > highest)
			highest = accuracyGraph[i];
		if ((accuracyGraph[i] < lowest) && 
			(accuracyGraph[i] != (float) 0))
			lowest = accuracyGraph[i];
	}
	
	/* print an ascii graph of learning progress */
	printf("\n\nLast 80 Epochs\n-----------------------------------------");
	printf("---------------------------------------\n");
	printf("%1.0f%%\n", highest * 100.0);
	for (int row = 20; row > 0; row--)
	{
		for (int col = 80 - 1; col >= 0; col--)
		{
			if (accuracyGraph[col] >= 
				((((float) (highest - lowest) / 20.0) * 
				(float) row) + lowest))
				printf("#");
			else
				printf(" ");
		}
		printf("\n");
	}
	printf("%1.0f%%\n", lowest * 100.0);
	printf("-------------------------------------------------------------");
	printf("-------------------\n");

	/* print time elapsed */
	printf("\nTraining complete after ");     
	if (trainingDuration < 60)
		printf("%d seconds.\n", (int) trainingDuration);
	else if (trainingDuration < 3600)
		printf("%d minutes & %d seconds.\n", 
		       (int) trainingDuration / 60, 
		       (int) trainingDuration % 60);
	else
		printf("%d hours, %d minutes & %d seconds.\n", 
		       (int) trainingDuration / 3600, 
		       ((int) trainingDuration % 3600) / 60, 
		       ((int) trainingDuration % 3600) % 60);
	
	/* print achieved achieved */
	if (accuracy > trainingCutoff)	
		printf("Reached above %1.0f%% accuracy after %d epochs.\n\n",
		       trainingCutoff * 100, epoch);	
	else
		printf("Reached %1.0f%% accuracy after %d epochs.\n\n",
		       accuracy * 100, epoch);	
}

/* test the network against the testing set */
void testNetwork ()
{
	printf("Testing on testing set...\n");
	
	int correct = 0;
	char testResult;
	Symbol testSymbol;
	
	/* count correct matches */
	for (int i = 0; i < testingSet.size; i++)
	{
		testSymbol = testingSet.symbol[i];
		testResult = forwardPass(testSymbol);

		if (testSymbol.type == testResult)
			correct++;
	}	
	/* print results */
	float accuracy = (((float) correct / testingSet.size) * 100);
	printf("%d / %d correct. %1.2f%% accuracy.\n\n", correct, 
	       testingSet.size, accuracy);
	       
}

/* do one epoch of the training process, returns accuracy */
float performTrainingEpoch ()
{
	/* list of numbers to use as training order and shuffle it */
	int *order = createOrder(trainingSet.size);
	shuffleNumbers(order, trainingSet.size);

	Symbol currentSymbol;
	
	/* k-fold numbers */
	int tenPercent = trainingSet.size / 10;
	int nintyPercent = 9 * (tenPercent);
	
	/* pass each symbol forward and back on first 90% */
	for (int i = 0; i < nintyPercent; i++)
	{
		currentSymbol = trainingSet.symbol[order[i]];
		forwardPass(currentSymbol);
		backwardProp(currentSymbol.type);		
	}
	
	/* test current results */
	int correct = 0;
	char testResult;
	Symbol testSymbol;
	
	/* run test on last 10% */
	for (int i = nintyPercent; i < trainingSet.size; i++)
	{
		testSymbol = trainingSet.symbol[order[i]];
		testResult = forwardPass(testSymbol);
				
		if (testSymbol.type == testResult)
			correct++;
	}
	
	free(order);
	
	/* print results */
	float accuracy = ((float) correct / tenPercent);
	printf("\t\t%d / %d correct. %1.2f%% accuracy.\n", correct, tenPercent, 
	       accuracy * 100);
	
	return accuracy;
}

/* pass a digit through the network, return interpreted value */
char forwardPass (Symbol s)
{
	/* load the digit into the input layer */
	for (int i = 0; i < network.structure[0]; i++)
		network.neuron[0][i].output = s.data[i];

	/* the magic happens here */
	for (int l = 1; l < network.layers; l++)
		for (int r = 0; r < network.structure[l]; r++)
		{
			network.neuron[l][r].input = 0;
			for (int s = 0; s < network.structure[l - 1]; s++)
				network.neuron[l][r].input += 
					network.neuron[l - 1][s].output * 
					network.connection[l - 1][s][r];

			network.neuron[l][r].output = 
			network.neuron[l][r].forward(network.neuron[l][r].input)
			;
		}
	
	char result = '0';
	float highestResponse = 0;
	
	/* find the highest response from the output */
	for (int o = 0; o < network.structure[network.layers - 1]; o++) 
	{
		if (network.neuron[network.layers - 1][o].output > 
			highestResponse)
		{
			highestResponse = 
				network.neuron[network.layers - 1][o].output;
			result = SYMBOL_TYPES[o];
		}	
	}
	
	return result;
}

/* propogate the error backwards through the network */
void backwardProp (char expected)
{		
	/* error for output neurons is set based on expected character */
	for (int o = 0; o < network.structure[network.layers - 1]; o++)
		if (o == (expected - '0'))
			network.neuron[network.layers - 1][o].error = 
				-(0.999999 - 
				network.neuron[network.layers - 1][o].output);
		else 
			network.neuron[network.layers - 1][o].error = 
				-(0.000001 - 
				network.neuron[network.layers - 1][o].output);		
				
	/* propogate the error backwards */
	for (int l = network.layers - 2; l > 0; l--)
		for (int s = 0; s < network.structure[l]; s++)
		{
			network.neuron[l][s].error = 0;
			for (int r = 0; r < network.structure[l + 1]; r++)
				network.neuron[l][s].error += 
					network.neuron[l + 1][r].error * 
					network.connection[l][s][r];
		}
	
	/* update each weight */
	for (int l = 0; l < network.layers - 1; l++)
		for (int s = 0; s < network.structure[l]; s++)
			for (int r = 0; r < network.structure[l + 1]; r++)
				network.connection[l][s][r] -= learningRate * 
					network.neuron[l + 1][r].error * 
					network.neuron[l + 1][r].backward
						(network.neuron[l + 1][r].input) 
						* network.neuron[l][s].output;
}

/* test all file functions on each file */
void testAllFiles () 
{
	FILE *file;

	char *variablePath = malloc(30 * sizeof(char));
	
	/* test on training data files */
	strcpy(variablePath, TRAIN_DATA_PATH);
	for (int i = 0; i < TYPE_COUNT; i++)
	{
		variablePath[TRAIN_PATH_OFFSET] = SYMBOL_TYPES[i];
		file = fopen(variablePath, "r");		
		printf("File: %s\n\tLines: %d\n\tLongest: %d\n\n", variablePath,
		       countLinesInFile(file), findLongestLineLength(file));
		fclose(file);
	}
	
	/* test on testing data files */
	strcpy(variablePath, TEST_DATA_PATH);
	for (int i = 0; i < TYPE_COUNT; i++)
	{
		variablePath[TEST_PATH_OFFSET] = SYMBOL_TYPES[i];
		file = fopen(variablePath, "r");		
		printf("File: %s\n\tLines: %d\n\tLongest: %d\n\n", variablePath,
		       countLinesInFile(file), findLongestLineLength(file));
		fclose(file);
	}
	
	free(variablePath);
}

/* create an array of integers from 0 to size for training order purposes */
int *createOrder (int size)
{
	int *order = malloc(size * sizeof(int));
	
	for (int i = 0; i < size; i++)
		order[i] = i;
	
	return order;
}

/* shuffle the order array */
void shuffleNumbers (int *list, int size)
{		
	for (int i = 0; i < size; i++)
		list[i] = i;

	int j, temp;

	/* fisher-yates shuffle */
	for (int i = size - 1; i > 0; i--)
	{
		j = (((float)rand() / RAND_MAX)) * i;
		temp = list[i];
		list[i] = list[j];
		list[j] = temp;
	}	
}

/* count how many lines are in a file, for allocating memory for digits */
int countLinesInFile (FILE *file)
{
	int lineCount = 0;
	int ch = 0;
	
	rewind(file);
	
	while (!feof(file))
	{
		ch = fgetc(file);
		if (ch == '\n')
			lineCount++;
	}
	
	rewind(file);
	return lineCount;
}

/* find the longest line in a file, also for digit memory allocation */
int findLongestLineLength (FILE *file)
{
	int longestLineLength = 0;
	int currentLineLength = 0;
	int ch = 0;
	
	rewind(file);

	while (!feof(file))
	{
		ch = fgetc(file);
		currentLineLength++;
		
		if (ch == '\n')
		{
			if (currentLineLength > longestLineLength)
				longestLineLength = currentLineLength;
			currentLineLength = 0;
		}
	}

	rewind(file);
	return longestLineLength;
}

/* function called at exit */
void cleanup ()
{
	freeAllSymbols();
	freeNetwork(network);
	printf("Exiting...\n\n");
}

/* free memory used by symbol sets */
void freeAllSymbols ()
{
	printf("Unloading symbols...\n");
	freeSymbolSet(trainingSet);
	freeSymbolSet(testingSet);
	printf("Done unloading symbols.\n\n");
}

/* free memory for individual digit sets */
void freeSymbolSet (SymbolSet set)
{
	int freedSymbols = 0;
	
	if (set.symbol)
	{
		for (int i = 0; i < set.size; i++)
			if (set.symbol[i].data)
			{
				free(set.symbol[i].data);
				freedSymbols++;
			}
		free(set.symbol);
	}
	
	printf("\t%d symbols freed.\n", freedSymbols);
}

/* free memory used by neural network structures */
void freeNetwork (NeuralNetwork net)
{
	/* free weights */
	for (int l = 0; l < net.layers - 1; l++) 
	{
		for (int s = 0; s < net.structure[l]; s++)
			if (net.connection[l][s])
				free(net.connection[l][s]);
				
		if (net.connection[l])
			free(net.connection[l]);
	}
	
	if (net.connection)
		free(net.connection);
	
	/* free neurons */
	for (int l = 0; l < net.layers; l++)
		if (net.neuron[l])
			free(net.neuron[l]);
	if (net.neuron)
		free(net.neuron);
	if (net.structure)
		free(net.structure);
}

