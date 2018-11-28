#include "configuration.h"
#include "data.h"
#include "dataio.h"
#include "simulation.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int main(int argc, char **argv) {
	srand(time(NULL));

	char fileName[128];

	{
		ChipConfiguration_t *config;

		int i;
		for (i = 1; i <= 4; ++i)
		{
			config = (ChipConfiguration_t *)0;

			sprintf(fileName, "c%d.txt", i);

			if (config = loadChipConfiguration(fileName))
			{
				printf("Running:\n");
				printChipConfiguration(config);

				DataSetMetaData_t *dsinfo;

				createDirectory("sets");

				int nBad = 0;

				printf("Generating data sets:\n");
				int x;
				for (x = 1; x <= config->numBatches; ++x)
				{
					dsinfo = (DataSetMetaData_t *)0;

					sprintf(fileName, "sets/%d", i);
					createDirectory(fileName);
					sprintf(fileName, "sets/%d/ds%d.txt", i, x);

					if (dsinfo = generateDataSet(fileName, config))
					{
						if (dsinfo->totBad > 0)
						{
							printf("\tCreate bad set batch #  %i, totBad =  %i total =  %i badpct =  %i\n", x, dsinfo->totBad, dsinfo->total, dsinfo->badpct);
							nBad++;
						}
					}
					else
					{
						printf("Could not generate data set '%s'. Aborting.\n", fileName);
					}
				}

				DataSet_t *ds;

				int nBadDetected = 0;

				printf("Analyzing data sets:\n");
				for (x = 1; x <= config->numBatches; ++x)
				{
					ds = (DataSet_t *)0;

					sprintf(fileName, "sets/%d/ds%d.txt", i, x);

					if (ds = readDataSet(fileName, config))
					{
						if (analyzeDataSet(ds, config))
						{
							printf("\tbatch #%d is bad\n", x);
							nBadDetected++;
						}
					}
					else
					{
						printf("Could not read data set '%s'. Aborting.\n", fileName);
					}
				}

				float base = 1.0 - (config->percentBadItems / 100.0);
				float Pf = pow(base, config->sampleSize);
				int Pb = (int) (100.0 * nBadDetected / (float) nBad);

				printf("Base = %f exponent = %d\n", base, config->sampleSize);
				printf("P(failure to detect bad item) = %f\n", Pf);
				printf("P(batch is good) = %f\n", 1.0 - Pf);
				printf("Percentage of bad batches detected = %d%%\n", Pb);

				freeChipConfiguration(config);
			}
			else
			{
				printf("Configuration files not prepared. Aborting.\n");
				return -1;
			}
		}
	}

	{
		ReadingConfiguration_t *config = (ReadingConfiguration_t *)0;

		strcpy(fileName, "readings.txt");

		if (config = loadReadingConfiguration(fileName))
		{
			int days;

			printf("How many days to simulate?\n");
			scanf("%d", &days);
			printf("\n");

			double analytical = calculateAnalyticalModel(config);

			int x = 0;
			while (analytical > config->maxes[x]) x++;

			printf("Analytical model: %.1f. Expected value is in the %d-%d/ml range.\n", analytical, config->mins[x], config->maxes[x]);

			double simulated = simulate(days, config);

			x = 0;
			while (simulated > config->maxes[x]) x++;

			printf("Simulation: %.1f. Expected value is in the %d-%d/ml range.\n", simulated, config->mins[x], config->maxes[x]);
		}
		else
		{
			printf("Configuration file not prepared. Aborting.\n");
			return -1;
		}
	}


	return 0;
}
