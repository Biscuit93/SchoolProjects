/* holds a single data vector with associated info */
typedef struct DataVector
{
	int type;
	int testData;
	float *value;
}
DataVector;

/* a set of data vectors with size and dimensionality */
typedef struct DataSet
{
	unsigned int size, dim;
	float *max;
	DataVector *vector;
}
DataSet;

/* a configuration holder for integer and floating point parameters */
typedef struct Config
{
	int configID;
	int intParam[ INTPARAM_COUNT ];
	float fltParam[ FLTPARAM_COUNT ];
}
Config;

/* a 2d lattice of weight vectors and associated data and configs */
typedef struct SelfOrganizingMap
{
	Config config;
	unsigned int dim;
	float radius, learnRate;
	float *input;
	float ***weight;
	unsigned int progress;
	int **class;
	int **match;
}
SelfOrganizingMap;
