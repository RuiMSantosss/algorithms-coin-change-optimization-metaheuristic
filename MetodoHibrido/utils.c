#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"
#include "algorithm.h"
#include <math.h>

// Inicializa��o do gerador de n�meros aleat�rios
void initRandom() {
	srand((unsigned)time(NULL));
}

// Leitura dos par�metros e dos dados do problema
// Par�metros de entrada: Nome do ficheiro e matriz a preencher com os dados dos objectos (peso e valor)
// Par�metros de sa�da: Devolve a estrutura com os par�metros
struct info initData(char *filename, float **coinsTypeArray) {
	struct  info x;
	FILE    *f;
	int     i;

	f = fopen(filename, "rt");
	if (!f)
	{
		printf("Erro ao abrir o ficheiro %s\n", filename);
		exit(1);
	}
	// Leitura dos par�metros do problema
	fscanf(f, "%d %f", &x.numGenes, &x.target);

	// Aloca dinamicamente para os valores das moedas
	*coinsTypeArray = malloc(sizeof(float) * x.numGenes);
	if (*coinsTypeArray == NULL) {
		printf("Erro ao alocar memória para coinsTypeArray\n");
		fclose(f);
		exit(1);
	}

	for (i = 0; i < x.numGenes; i++) {
		fscanf(f, "%f", &(*coinsTypeArray)[i]);
	}

	x.populationSize = 20;  // Exemplo
	x.probabilityMutation = 0.1;
	x.probabilityRecombination = 0.5;
	x.tourneySize = 2;
	x.numGenerations = 10000;
	x.rho = 100;

	fclose(f);
	return x;
}


// Simula o lan�amento de uma moeda, retornando o valor 0 ou 1
int flip()
{
	if ((((float)rand()) / RAND_MAX) < 0.5)
		return 0;
	else
		return 1;
}

// Criacao da populacao inicial. O vector e alocado dinamicamente
// Par�metro de entrada: Estrutura com par�metros do problema
// Par�metro de sa�da: Preenche da estrutura da popula��o apenas o vector bin�rio com os elementos que est�o dentro ou fora da mochila
pChromosome initPopulation(struct info data, float *coinsTypeArray) {
	int i, j;
	pChromosome population;

	// Aloca espaço para a população
	population = malloc(sizeof(chromosome) * data.populationSize);
	if (population == NULL) {
		printf("Erro na alocacao de memoria para a populacao\n");
		exit(1);
	}

	// Gera cada indivíduo da população
	for (i = 0; i < data.populationSize; i++) {
		// Aloca e inicializa o vetor p de cada cromossomo
		population[i].p = malloc(data.numGenes * sizeof(int));
		if (population[i].p == NULL) {
			printf("Erro na alocacao de memoria para p[%d]\n", i);
			exit(1);
		}

		// Inicializa o vetor de moedas para 0
		for (j = 0; j < data.numGenes; j++) {
			population[i].p[j] = 0;
		}

		// Inicializa a soma
		float soma = 0;

		// Gera uma solução inicial para o indivíduo
		while (soma < data.target) {
			// Escolhe aleatoriamente um tipo de moeda
			int indice = random_l_h(0, data.numGenes - 1);
			float novoValor = soma + coinsTypeArray[indice];

			// Gera um número aleatório para decidir se podemos adicionar a moeda
			float probabilidade = rand_01();

			if (novoValor > data.target && probabilidade > 0.5) {
				break; // Permite parar com a soma abaixo ou igual ao valor-alvo em 50% dos casos
			}

			// Incrementa a quantidade da moeda selecionada
			population[i].p[indice]++;
			soma = novoValor; // Atualiza a soma total
		}

		// Inicializa o fitness e a validade do indivíduo
		population[i].fitness = 0;
		population[i].valid = 0;
	}

	return population;
}

// Actualiza a melhor solu��o encontrada
// Par�metro de entrada: populacao actual (pop), estrutura com par�metros (d) e a melhor solucao encontrada at� a gera��oo imediatamente anterior (best)
// Par�metro de sa�da: a melhor solucao encontrada at� a gera��o actual
chromosome getBest(pChromosome population, struct info data, chromosome best) {
	int i;

	best = population[0]; // Inicializa com o primeiro indivíduo

	for (i = 1; i < data.populationSize; i++) {
		if (population[i].fitness < best.fitness) {  // Menor fitness = menos moedas
			best = population[i];
		}
	}

	return best;
}

// Devolve um valor inteiro distribuido uniformemente entre min e max
int random_l_h(int min, int max)
{
	return min + rand() % (max-min+1);
}

// Devolve um valor real distribuido uniformemente entre 0 e 1
float rand_01()
{
	return ((float)rand())/RAND_MAX;
}

// Escreve uma solu��o na consola
// Par�metro de entrada: populacao actual (pop) e estrutura com par�metros (d)
void writeBest(chromosome x, struct info data) {
	int i;

	printf("\nBest individual: %4.1f\n", x.fitness);
	for (i = 0; i < data.numGenes; i++) {
		printf("%d ", x.p[i]);
	}
	putchar('\n');
}

