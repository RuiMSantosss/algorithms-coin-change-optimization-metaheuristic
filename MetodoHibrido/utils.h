

#ifndef ALGORITMOEVOLUTIVO_UTILS_H
#define ALGORITMOEVOLUTIVO_UTILS_H

#include "algorithm.h"

void initRandom();

struct info initData(char *filename, float **coinsTypeArray);

int flip();

pChromosome initPopulation(struct info data, float *coinsTypeArray);

chromosome getBest(pChromosome pop, struct info d, chromosome best);

void writeBest(chromosome x, struct info data);

int random_l_h(int min, int max);

float rand_01();
#endif //ALGORITMOEVOLUTIVO_UTILS_H
