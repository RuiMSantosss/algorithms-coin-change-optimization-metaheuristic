

#ifndef ALGORITMOEVOLUTIVO_ALGORITHM_H
#define ALGORITMOEVOLUTIVO_ALGORITHM_H

#define MAX_OBJ 1000

struct info
{
	// Tamanho da população
	int     populationSize;
	// Probabilidade de mutação
	float   probabilityMutation;
	// Probabilidade de recombinação
	float   probabilityRecombination;
	// Tamanho do torneio para seleção do pai da próxima geração
	int     tourneySize;
	// Constante para avaliação com penalização
	float   rho;
	// Número de tipos de moedas (genes)
	int     numGenes;
	// Capacidade da mochila
	float     target;
	// Número de gerações
	int     numGenerations;
	// Array para os valores das moedas (dinamicamente alocado)
	float   *coinValues;
};

typedef struct individual chromosome, *pChromosome;

struct individual
{
	// Solução (objetos que estão dentro da mochila)
	int *p;
	// Valor da qualidade da solução
	float   fitness;
	// 1 se for uma solução válida e 0 se não for
	int     valid;
};

void substitute(int a[], int b[], int n);

void tournament(pChromosome population, struct info d, pChromosome parents);

void tournament_n(pChromosome population, struct info data, pChromosome parents);

void generateNeighbour(int solution[], int newSol[],int numberCoinsType, float coinsValueToReach, float *coinsTypeArray);

int trepa_colinas(int solution[], float *coinsTypeArray, struct info data,int numberCoinsType, float coinsValueToReach, int num_iter);   //_probabilistico

void geneticOperators(pChromosome parents, struct info d, pChromosome offspring);

void crossover(pChromosome parents, struct info d, pChromosome offspring);

void crossover_two_Points(pChromosome parents, struct info data, pChromosome offspring);

void crossover_uniforme(pChromosome parents, struct info data, pChromosome offspring);

void mutation(pChromosome offspring, struct info data);

void mutationSwap(pChromosome offspring, struct info data);

void repair(pChromosome population, struct info data, float *coinsTypeArray);

float calculateSum(int solution[], struct info data, float *coinsTypeArray);

void mutationSwap(pChromosome offspring, struct info data);
#endif //ALGORITMOEVOLUTIVO_ALGORITHM_H
