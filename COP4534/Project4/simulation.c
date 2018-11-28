/*
 *
 */

#include "configuration.h"
#include "simulation.h"

double calculateAnalyticalModel(ReadingConfiguration_t *config)
{
	double total = 0.0;
	int i;
	for (i = 0; i < config->ranges; ++i)
	{
		total += (config->values[i] / (double) config->totalReadings) * ((config->mins[i] + config->maxes[i]) / 2.0);
	}

	return total;
}

double simulate(int days, ReadingConfiguration_t *config)
{
	int probs[config->ranges];

	int i;
	for (i = 0; i < config->ranges - 1; ++i)
	{
		probs[i] = (int) (100.0 * config->values[i] / (float) config->totalReadings);
	}

	probs[i] = 100;

	double total = 0.0;
	int val, x;
	for (i = 0; i < days; ++i)
	{
		val = rand() % 100;
		x = 0;

		while (val > probs[x]) x++;

		total += ((config->mins[x] + config->maxes[x]) / 2.0);
	}

	return total / days;
}
