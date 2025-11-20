#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include <math.h>
#include "algorithm.h"
#include "funcao.h"

int main() {
    char fileName[100];
    struct info evolutiveAlgorithmParameters;
    pChromosome population = NULL, parents = NULL;;
    chromosome bestRun, bestEver;
    int runs, i, j, genAtual, invalid;
    float *coinsTypeArray;
    float  MBF = 0;

    do {
        printf("Numero de repeticoes: ");
        scanf("%d", &runs);
    } while(runs <= 0);

    printf("\nNome do Ficheiro:");
    scanf("%s", fileName);
    initRandom();

    evolutiveAlgorithmParameters = initData(fileName, &coinsTypeArray);
    if (evolutiveAlgorithmParameters.numGenes <= 0 || coinsTypeArray == NULL) {
        printf("Erro na inicialização dos dados ou alocação de coinsTypeArray.\n");
        return -1;
    }
    printf("Target: %.2f\n", evolutiveAlgorithmParameters.target);
    printf("Moedas: ");
    for (int j = 0; j < evolutiveAlgorithmParameters.numGenes; j++) {
        printf("%.2f ", coinsTypeArray[j]);
    }
    printf("\n");

    for (i = 0; i < runs; i++) {
        printf("\nRepeticao %d\n", i + 1);

        population = initPopulation(evolutiveAlgorithmParameters, coinsTypeArray);

        evaluate(population, evolutiveAlgorithmParameters, coinsTypeArray);

        genAtual = 1;

        bestRun = population[0];

        bestRun = getBest(population, evolutiveAlgorithmParameters, bestRun);

        parents = malloc(sizeof(chromosome)*evolutiveAlgorithmParameters.populationSize);
        if (parents==NULL)
        {
          printf("Erro na alocacao de memoria\n");
          exit(1);
        }

        while (genAtual <= evolutiveAlgorithmParameters.numGenerations){
          //tournament(population, evolutiveAlgorithmParameters, parents);

          tournamentWithNSize(population, evolutiveAlgorithmParameters, parents);

          geneticOperators(parents, evolutiveAlgorithmParameters, population);

          repair(population,evolutiveAlgorithmParameters, coinsTypeArray);

          evaluate(population, evolutiveAlgorithmParameters, coinsTypeArray);

          bestRun = getBest(population, evolutiveAlgorithmParameters, bestRun);
          genAtual++;
        }

        for (invalid = 0, j = 0; j <evolutiveAlgorithmParameters.populationSize; j++) {
          if (population[i].valid == 0) {
              invalid++;
          }
        }
        //printf("invalidos: %d\n PopulationSize: %d\n", invalid, evolutiveAlgorithmParameters.populationSize);
        //printf("\nRepeticao %d:", i);
        writeBest(bestRun, evolutiveAlgorithmParameters);
        printf("\nPercentagem Invalidos: %f\n", 100 * ((float)invalid / evolutiveAlgorithmParameters.populationSize));
        MBF += bestRun.fitness;
        if (i == 0 || bestRun.fitness < bestEver.fitness) {
          bestEver = bestRun;
        }
        // Liberta a mem�ria usada
        free(parents);
        free(population);
    }
    printf("\n\nMBF: %f\n", MBF/i);
    printf("\nMelhor solucao encontrada");
    writeBest(bestEver, evolutiveAlgorithmParameters);
    return 0;
}
