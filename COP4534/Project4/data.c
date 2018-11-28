/*
 *
 */

#include "data.h"
#include "configuration.h"
#include <stdio.h>
#include <stdlib.h>

DataSetMetaData_t * createEmptyDataSetMetaData()
{
	DataSetMetaData_t *dsinfo = (DataSetMetaData_t *)malloc(sizeof(DataSetMetaData_t));

	dsinfo->totBad = 0;

	dsinfo->total = 0;

	return dsinfo;
}

DataSet_t * createEmptyDataSet(ChipConfiguration_t *config)
{
	DataSet_t *ds = (DataSet_t *)malloc(sizeof(DataSet_t));

	ds->total = 0;

	ds->data = (char *)malloc(sizeof(char) * config->numBatchItems);

	return ds;
}

DataSetMetaData_t * generateDataSet(char *fileName, ChipConfiguration_t *config)
{
	DataSetMetaData_t *dsinfo = (DataSetMetaData_t *)0;

	FILE *file = fopen(fileName, "w+");

	if (file)
	{
		dsinfo = createEmptyDataSetMetaData();

		int i;

		if (rand() % 100 < config->percentBadBatches)
		{
			for (i = 0; i < config->numBatchItems; ++i)
			{
				if (rand() % 100 < config->percentBadItems)
				{
					fprintf(file, "b\n");
					dsinfo->totBad++;
				}
				else
				{
					fprintf(file, "g\n");
				}
			}
		}
		else
		{
			for (i = 0; i < config->numBatchItems; ++i)
			{
				fprintf(file, "g\n");
			}
		}

		fclose(file);

		dsinfo->total = config->numBatchItems;

		dsinfo->badpct = (int) (100.0 * dsinfo->totBad / ((float) dsinfo->total));
	}
	else
	{
		printf("Could not create dataset '%s'. Aborting.\n", fileName);
	}

	return dsinfo;
}

DataSet_t * readDataSet(char *fileName, ChipConfiguration_t *config)
{
	DataSet_t *ds = (DataSet_t *)0;

	FILE *file = fopen(fileName, "r");

	char buffer[3];

	if (file)
	{
		ds = createEmptyDataSet(config);

		while (fgets(buffer, 3, file) && ds->total < config->numBatchItems)
		{
			sscanf(buffer, "%c", &ds->data[ds->total]);

			ds->total++;

			ds->data = (char *)realloc(ds->data, sizeof(char) * (ds->total + 1));
		}

		fclose(file);

		if (ds->total < config->numBatchItems)
		{
			printf("Cannot read incomplete data set '%s'! Expected %d items, only found %d.\n", config->numBatchItems, ds->total);
		}
	}
	else
	{
		printf("Cannot open nonexistent data set '%s'.\n", fileName);
	}

	return ds;
}

int analyzeDataSet(DataSet_t *dataset, ChipConfiguration_t *config)
{
	if (dataset->total != config->numBatchItems)
	{
		printf("Data set and configuration mismatch; dataset has %d items, config expects %d. Returning 0.\n", dataset->total, config->numBatchItems);
		return 0;
	}

	int i, idx;
	for (i = 0; i < config->sampleSize; ++i)
	{
		idx = rand() % dataset->total;

		if (dataset->data[idx] == 'b')
		{
			return 1;
		}
	}

	return 0;
}
