
#include "algorithm.h"
#include <stdlib.h>
#include <stdio.h>
#include "funcao.h"
#include <math.h>

float evaluateIndividual(int solution[], struct info data, float *coinsTypeArray, int *valid) {
    int i, totalCoins = 0;
    float sum_value = 0;

    // Calcula o total de moedas e o valor gerado pela solução
    for (i = 0; i < data.numGenes; i++) {
        totalCoins += solution[i];
        sum_value += solution[i] * coinsTypeArray[i];
    }

    // Verifica se a solução é válida
    if (fabs(sum_value - data.target) < 0.00001) { // Ajusta tolerância
        *valid = 1;  // Solução válida
        return (float)totalCoins;  // Fitness: menor número de moedas
    } else {
        *valid = 0;  // Solução inválida
        return data.rho * fabs(data.target - sum_value);  // Penalização
    }
}

void evaluate(pChromosome population, struct info data, float *coinsTypeArray) {
    int i;

    for (i=0; i<data.populationSize; i++) {
        population[i].fitness = evaluateIndividual(population[i].p, data, coinsTypeArray, &population[i].valid);
    }
}