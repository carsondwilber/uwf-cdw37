/*
 *
 */

#include "configuration.h"
#include <stdio.h>
#include <stdlib.h>

ChipConfiguration_t * createEmptyChipConfiguration()
{
	ChipConfiguration_t *config = (ChipConfiguration_t *)malloc(sizeof(ChipConfiguration_t));

	return config;
}

ReadingConfiguration_t * createEmptyReadingConfiguration()
{
	ReadingConfiguration_t *config = (ReadingConfiguration_t *)malloc(sizeof(ReadingConfiguration_t));

	return config;
}

ChipConfiguration_t * loadChipConfiguration(char *fileName)
{
	ChipConfiguration_t *config = (ChipConfiguration_t *)0;

	FILE *file = fopen(fileName, "r");

	if (file)
	{
		config = createEmptyChipConfiguration();

		char buffer[256];

		if (fgets(buffer, 256, file))
		{
			sscanf(buffer, "%d", &config->numBatches);

			if (fgets(buffer, 256, file))
			{
				sscanf(buffer, "%d", &config->numBatchItems);

				if (fgets(buffer, 256, file))
				{
					sscanf(buffer, "%d", &config->percentBadBatches);

					if (fgets(buffer, 256, file))
					{
						sscanf(buffer, "%d", &config->percentBadItems);

						if (fgets(buffer, 256, file))
						{
							sscanf(buffer, "%d", &config->sampleSize);
						}
						else
						{
							printf("Cannot read incomplete configuration! Need sample size.\n");
							config = (ChipConfiguration_t *)0;
						}
					}
					else
					{
						printf("Cannot read incomplete configuration! Need percentage of bad items in a bad batch.\n");
						config = (ChipConfiguration_t *)0;
					}
				}
				else
				{
					printf("Cannot read incomplete configuration! Need percentage of bad batches.\n");
					config = (ChipConfiguration_t *)0;
				}
			}
			else
			{
				printf("Cannot read incomplete configuration! Need number of items in batch.\n");
				config = (ChipConfiguration_t *)0;
			}
		}
		else
		{
			printf("Cannot read incomplete configuration! Need number of batches.\n");
			config = (ChipConfiguration_t *)0;
		}
	}
	else
	{
		printf("Cannot read nonexistent configuration file, '%s'!\n", fileName);
	}

	return config;
}

ReadingConfiguration_t * loadReadingConfiguration(char *fileName)
{
	ReadingConfiguration_t *config = (ReadingConfiguration_t *)0;

	FILE *file = fopen(fileName, "r");

	if (file)
	{
		config = createEmptyReadingConfiguration();

		char buffer[256];

		if (fgets(buffer, 256, file))
		{
			sscanf(buffer, "%d", &config->totalReadings);

			if (fgets(buffer, 256, file))
			{
				sscanf(buffer, "%d", &config->ranges);

				config->mins = (int *)malloc(sizeof(int) * config->ranges);
				config->maxes = (int *)malloc(sizeof(int) * config->ranges);
				config->values = (int *)malloc(sizeof(int) * config->ranges);

				int range = 0;
				while (range < config->ranges && fgets(buffer, 256, file))
				{
					sscanf(buffer, "%d-%d/ml: %d", &config->mins[range], &config->maxes[range], &config->values[range]);
					++range;
				}

				if (range < config->ranges)
				{
					printf("Cannot read incomplete configuration! Not all value ranges found in file; only found %d.\n", config->ranges - range);
					config = (ReadingConfiguration_t *)0;
				}
			}
			else
			{
				printf("Cannot read incomplete configuration! Need number of items in batch.\n");
				config = (ReadingConfiguration_t *)0;
			}
		}
		else
		{
			printf("Cannot read incomplete configuration! Need number of batches.\n");
			config = (ReadingConfiguration_t *)0;
		}

		fclose(file);
	}
	else
	{
		printf("Cannot read nonexistent configuration file, '%s'!\n", fileName);
	}

	return config;
}

void printChipConfiguration(ChipConfiguration_t *config)
{
	printf("\t%-50s%10d\n", "Number of batches", config->numBatches);
	printf("\t%-50s%10d\n", "Number of items in each batch", config->numBatchItems);
	printf("\t%-50s%9d%%\n", "Percentage of batches containing bad items", config->percentBadBatches);
	printf("\t%-50s%9d%%\n", "Percentage of items that are bad in a bad batch", config->percentBadItems);
	printf("\t%-50s%10d\n", "Items sampled from each set", config->sampleSize);
}

void printReadingConfiguration(ReadingConfiguration_t *config)
{

}

void freeChipConfiguration(ChipConfiguration_t *config)
{
	free(config);
}

void freeReadingConfiguration(ReadingConfiguration_t *config)
{
	free(config->mins);
	free(config->maxes);
	free(config->values);

	free(config);
}
