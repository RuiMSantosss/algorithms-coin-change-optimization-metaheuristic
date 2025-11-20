#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include <math.h>
#include "algorithm.h"
#include "funcao.h"

int main()
{
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

	printf("\nNome do Ficheiro: ");
	scanf("%s", fileName);
	initRandom();
    // Preenche a matriz com dados dos objectos (peso e valor) e a estrutura EA_param que foram definidos no ficheiro de input
	evolutiveAlgorithmParameters = initData(fileName, &coinsTypeArray);

	printf("Target: %.2f\n", evolutiveAlgorithmParameters.target);
	printf("Moedas: ");
	for (int j = 0; j < evolutiveAlgorithmParameters.numGenes; j++) {
		printf("%.2f ", coinsTypeArray[j]);
	}
	printf("\n");
	// Faz um ciclo com o n�mero de execu��es definidas
	for (i = 0; i < runs; i++) {
		printf("\nRepeticao %d\n",i+1);
        // Gera��o da popula��o inicial
		population = initPopulation(evolutiveAlgorithmParameters, coinsTypeArray);
		 // Avalia a popula��o inicial
		evaluate(population, evolutiveAlgorithmParameters, coinsTypeArray);

		if(hibrido_flag == 1) {
			for(i = 0; i< evolutiveAlgorithmParameters.populationSize;i++) {
				population[i].fitness = trepa_colinas(population[i].p, coinsTypeArray,struct info data,int numberCoinsType, float coinsValueToReach, evolutiveAlgorithmParameters, 1000);
			}
		}
		genAtual = 1;
		// Como ainda n�o existe, escolhe-se como melhor solu��o a primeira da popula��o (poderia ser outra qualquer)
		bestRun = population[0];

		bestRun = getBest(population, evolutiveAlgorithmParameters, bestRun);

		parents = malloc(sizeof(chromosome)*evolutiveAlgorithmParameters.populationSize);
		if (parents==NULL)
		{
			printf("Erro na alocacao de memoria\n");
			exit(1);
		}
		// Ciclo de optimiza��o
		while (genAtual <= evolutiveAlgorithmParameters.numGenerations){
			tournament(population, evolutiveAlgorithmParameters, parents);

			geneticOperators(parents, evolutiveAlgorithmParameters, population);

			//repair(population,evolutiveAlgorithmParameters, coinsTypeArray);

			evaluate(population, evolutiveAlgorithmParameters, coinsTypeArray);

			bestRun = getBest(population, evolutiveAlgorithmParameters, bestRun);

			genAtual++;
		}
		if(hibrido_flag == 2) {
			for(i = 0; i< EA_param.popsize;i++) {
				population[i].fitness = trepa_colinas(population[i].p, mat, EA_param, 1000);
			}
		}

		for (invalid = 0, j = 0; j <evolutiveAlgorithmParameters.populationSize; j++) {
			if (population[i].valid == 0) {
				invalid++;
			}

		writeBest(bestRun, evolutiveAlgorithmParameters);
		printf("\nPercentagem Invalidos: %f\n", 100 * ((float)invalid / evolutiveAlgorithmParameters.populationSize));
		MBF += bestRun.fitness;
		if (i == 0 || bestRun.fitness < bestEver.fitness) {
			bestEver = bestRun;
		}

		free(parents);
		free(population);
		}
		printf("\n\nMBF: %f\n", MBF/i);
		printf("\nMelhor solucao encontrada");
		writeBest(bestEver, evolutiveAlgorithmParameters);
		return 0;
	}
