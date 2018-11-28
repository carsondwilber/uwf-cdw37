/*
 *
 */

#ifndef DATA_H_
#define DATA_H_

#include "configuration.h"

struct DataSetMetaData_st
{
	int totBad;
	int total;
	int badpct;
};

typedef struct DataSetMetaData_st DataSetMetaData_t;

struct DataSet_st
{
	char * data;
	int total;
};

typedef struct DataSet_st DataSet_t;

DataSetMetaData_t * createEmptyDataSetMetaData();

DataSet_t * createEmptyDataSet(ChipConfiguration_t *config);

DataSetMetaData_t * generateDataSet(char *fileName, ChipConfiguration_t *config);

DataSet_t * readDataSet(char *fileName, ChipConfiguration_t *config);

int analyzeDataSet(DataSet_t *dataset, ChipConfiguration_t *config);

#endif /* DATA_H_ */
