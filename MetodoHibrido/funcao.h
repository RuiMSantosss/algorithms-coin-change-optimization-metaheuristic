

#ifndef ALGORITMOEVOLUTIVO_FUNCAO_H
#define ALGORITMOEVOLUTIVO_FUNCAO_H

int evaluateIndividual(int sol[], struct info d, float *coinsTypeArray, int *valid);

void evaluate(pChromosome population, struct info data, float *coinsTypeArray);

#endif //ALGORITMOEVOLUTIVO_FUNCAO_H
