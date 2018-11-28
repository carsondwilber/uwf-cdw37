/*
 *
 */

#ifndef SIMULATION_H_
#define SIMULATION_H_

#include "configuration.h"

double calculateAnalyticalModel(ReadingConfiguration_t *config);

double simulate(int days, ReadingConfiguration_t *config);

#endif /* SIMULATION_H_ */
