#include <stdio.h>
#include <stdlib.h>
#include "algorithm.h"
#include "utils.h"
#include <math.h>
#include "funcao.h"

void substitute(int a[], int b[], int n) {
	int i;
	for(i=0; i<n; i++) {
		a[i] = b[i];
	}
}
void tournament(pChromosome population, struct info data, pChromosome parents) {
	int i, x1, x2;

	for (i = 0; i < data.populationSize; i++) {
		x1 = random_l_h(0, data.populationSize - 1);
		do {
			x2 = random_l_h(0, data.populationSize - 1);
		} while (x1 == x2);

		// Seleciona o indivíduo com menor fitness (menos moedas)
		if (population[x1].fitness < population[x2].fitness) {
			parents[i] = population[x1];
		} else {
			parents[i] = population[x2];
		}
	}
}

void tournament_n(pChromosome population, struct info data, pChromosome parents)
{
	int i, new_x,contador = 0, to_discard = 0;
	int tournament_size = data.tourneySize;
	int x[tournament_size];
	float this_fitness = 0.0, best_fitness = 0;

	// Realiza popsize torneios
	for(i = 0; i < data.populationSize ;i++)
	{
		x[0]= random_l_h(0, data.populationSize-1);
		best_fitness = population[x[0]].fitness;

		while(contador < tournament_size) {
			to_discard = 0;
			new_x = random_l_h(0, data.populationSize-1);
			//ver se o novo ponto ja esta no torneio
			for(int j = 0; j < contador; j++) {
				if(x[j] == new_x) {
					to_discard = 1;
				}
			}
			if(to_discard==0) {
				x[contador] = new_x;
				this_fitness = population[new_x].fitness;

				if(this_fitness > best_fitness) { //problema de maximizacao
					parents[i] = population[new_x];
					best_fitness = this_fitness;
				}
				contador++;
			}

		}
	}
}

void generateNeighbour(int solution[], int newSol[],int numberCoinsType, float coinsValueToReach, float *coinsTypeArray) {
	substitute(newSol, solution, numberCoinsType);

	// Escolhe aleatoriamente um índice para modificar
	int idx = random_l_h(0, numberCoinsType - 1);
	if (rand() % 2 == 0) {
		newSol[idx]++; // Incrementa moedas no índice selecionado
	} else if (newSol[idx] > 0) {
		newSol[idx]--; // Decrementa moedas no índice selecionado
	}

	// Cálculo da soma total
	float sum = 0;
	for (int i = 0; i < numberCoinsType; i++) {
		sum += newSol[i] * coinsTypeArray[i];
	}

	// Ajustar se a soma ultrapassar o valor alvo
	if (sum > coinsValueToReach) {
		float excess = sum - coinsValueToReach; // Calcula o excesso
		for (int i = numberCoinsType - 1; i >= 0; i--) { // Começa pelas maiores moedas
			while (excess > 0 && newSol[i] > 0) {
				newSol[i]--; // Remove uma moeda
				excess -= coinsTypeArray[i]; // Atualiza o excesso
			}
			if (excess <= 0) break; // Para se o excesso for eliminado
		}
	}else if (sum < coinsValueToReach) {
		float deficit = coinsValueToReach - sum; // Calcula o déficit
		for (int i = numberCoinsType - 1; i >= 0; i--) { // Começa pelas maiores moedas
			while (deficit > 0) {
				newSol[i]++; // Adiciona uma moeda
				deficit -= coinsTypeArray[i]; // Atualiza o déficit
				if (deficit <= 0) break; // Para se o déficit for resolvido
			}
		}
	}
}

int trepa_colinas(int solution[], float *coinsTypeArray, struct info data,int numberCoinsType, float coinsValueToReach, int num_iter)   //_probabilistico
{
	int *newSol, i, dummy;
	float cost, costNeighbour;

	newSol = malloc(sizeof(int)*data.numGenes);
	if(newSol == NULL)
	{
		printf("Erro na alocacao de memoria");
		exit(1);
	}

	// Avalia solucao inicial
	cost = evaluateIndividual(solution,data, coinsTypeArray, &dummy);

	for(i = 0; i < num_iter; i++)
	{
		// Gera vizinho
		generateNeighbour(solution, newSol, numberCoinsType, coinsValueToReach, coinsTypeArray);
		// Avalia vizinho
		costNeighbour = evaluateIndividual(newSol,data, coinsTypeArray, &dummy);
		// Aceita vizinho se o custo diminuir (problema de maximizacao)
		if(costNeighbour >= cost) // com igualdade <= -> segunda tabela
		{
			substitute(solution, newSol, data.numGenes);
			cost = costNeighbour;
		}
	}
	free(newSol);
	return cost;
}

// Operadores geneticos a usar na gera��o dos filhos
// Par�metros de entrada: estrutura com os pais (parents), estrutura com par�metros (d), estrutura que guardar� os descendentes (offspring)
void geneticOperators(pChromosome parents, struct info data, pChromosome offspring) {
	// Recombinação com um ponto de corte
	crossover(parents, data, offspring);
	// Mutação binária
	mutation(offspring, data);
	// Mutação com troca
	//mutationSwap(offspring, data);
}

// Preenche o vector descendentes com o resultado das opera��es de recombina��o
// Par�metros de entrada: estrutura com os pais (parents), estrutura com par�metros (d), estrutura que guardar� os descendentes (offspring)
void crossover(pChromosome parents, struct info data, pChromosome offspring) {
	int i, j, point;

	for (i = 0; i < data.populationSize; i += 2) {  // Avança de 2 em 2
		if (rand_01() < data.probabilityRecombination) {
			point = random_l_h(0, data.numGenes - 1);  // Escolhe um ponto de corte
			for (j = 0; j < point; j++) {  // Primeira parte do crossover
				offspring[i].p[j] = parents[i].p[j];
				offspring[i + 1].p[j] = parents[i + 1].p[j];
			}
			for (j = point; j < data.numGenes; j++) {  // Segunda parte do crossover
				offspring[i].p[j] = parents[i + 1].p[j];
				offspring[i + 1].p[j] = parents[i].p[j];
			}
		} else {
			offspring[i] = parents[i];  // Sem crossover, copia diretamente o pai
			offspring[i + 1] = parents[i + 1];
		}
	}
}

void crossover_two_Points(pChromosome parents, struct info data, pChromosome offspring)
{
	int i, j, point1, point2, dummy;

	for (i = 0; i < data.populationSize; i+=2) //percorre a populacao de pais em 2 em 2 ( sao 2 pais)
	{
		if (rand_01() < data.probabilityRecombination) {			//pr é a Probabilidade de recombinação
			point1 = random_l_h(0, data.numGenes - 1);
			point2 = random_l_h(0, data.numGenes - 1);
			while(point2 == point1) // para os 2 pontos serem diferentes
			{
				point2 = random_l_h(0, data.numGenes - 1);
			}
			if(point2 < point1) {	//ordenar
				dummy = point1;
				point1 = point2;
				point2 = dummy;
			}
			//inicializa os filhos iguais aos pais
			offspring[i].p[j]= parents[i].p[j];
			offspring[i+1].p[j] = parents[i+1].p[j];

			//Alterar a parte central
			for (j = point1; j < point2; j++)//troca
			{
				offspring[i].p[j]= parents[i+1].p[j];
				offspring[i+1].p[j] = parents[i].p[j];
			}
		}
		else {
			offspring[i]= parents[i];
			offspring[i+1] = parents[i+1];
		}
	}
}

void crossover_uniforme(pChromosome parents, struct info data, pChromosome offspring) {
	int i, j;

	for (i = 0; i < data.populationSize; i+=2) { // percorre a populacao de pais de 2 em 2(sao 2 pais)
		//inicializa os filhos iguais aos pais
		offspring[i]= parents[i];
		offspring[i+1] = parents[i+1];

		for (j=0; j<data.numGenes; j++) // percorre as posicoes dos pais
		{
			if(parents[i].p[j] != parents[i+1].p[j]) {  // se os pais from diferentes
				if(rand_01() < data.probabilityRecombination) { //pr e a probabilidade de recombinacao
					offspring[i].p[j] = parents[i+1].p[j];
					offspring[i+1].p[j] = parents[i].p[j];
				}
			}
		}
	}
}

// Mutacao binaria com varios pontos de mutacao
// Parametros de entrada: estrutura com os descendentes (offspring) e estrutura com parametros (d)
void mutation(pChromosome offspring, struct info data) {
	int i, j;

	// Para cada indivíduo na população de descendentes
	for (i = 0; i < data.populationSize; i++) {
		// Para cada gene (moeda) de cada indivíduo
		for (j = 0; j < data.numGenes; j++) {
			// Aplica mutação com a probabilidade de mutação especificada
			if (rand_01() < data.probabilityMutation) {
				if(offspring[i].p[j] > 0) {
					offspring[i].p[j] = (offspring[i].p[j])--;
				}else if(offspring[i].p[j] == 0) {
					offspring[i].p[j] = (offspring[i].p[j])++;
				}
			}
		}
	}
}

void mutationSwap(pChromosome offspring, struct info data) {
	int i, obj1, obj2;

	// Para cada indivíduo na população
	for (i = 0; i < data.populationSize; i++) {
		// Aplica mutação com uma certa probabilidade
		if (rand_01() < data.probabilityMutation) {
			// Encontra um gene (moeda) aleatório com valor maior que 0
			obj1 = -1;
			for (int attempt = 0; attempt < data.numGenes; attempt++) {
				obj1 = random_l_h(0, data.numGenes - 1);
				if (offspring[i].p[obj1] > 0) {
					break; // Encontrei um gene válido
				}
			}

			// Se não encontrou nenhum gene válido (todas as moedas são 0), pula para o próximo indivíduo
			if (obj1 == -1 || offspring[i].p[obj1] <= 0) {
				continue;
			}

			// Encontra outro gene aleatório (diferente de obj1)
			do {
				obj2 = random_l_h(0, data.numGenes - 1);
			} while (obj2 == obj1);

			// Realiza a troca: remove uma moeda de obj1 e adiciona a obj2
			offspring[i].p[obj1]--;  // Remove uma moeda de obj1
			offspring[i].p[obj2]++;  // Adiciona uma moeda a obj2
		}
	}
}

float calculateSum(int solution[], struct info data, float *coinsTypeArray) {
	int i;
	float sumWeight = 0;

	for(i = 0; i < data.numGenes; i++) {
		if(solution[i] > 0) {
			sumWeight += (float)solution[i] * coinsTypeArray[i];
		}
	}
	//printf("sumWeight: %f\n", sumWeight);
	return sumWeight;
}

void repair(pChromosome population, struct info data, float *coinsTypeArray) {
	int i, j, stopAdd, stopRem;
	float currentSum;

	for (i = 0; i < data.populationSize; i++) {
		currentSum = calculateSum(population[i].p, data, coinsTypeArray);
		// Ajusta a solução enquanto a soma não estiver próxima do valor alvo
		while (fabs(currentSum - data.target) > 0.0001) {
			if (currentSum < data.target) {
				stopAdd = 0;
				// Adiciona a moeda de MAIOR valor que possa ser usada
				for (j = data.numGenes - 1; stopAdd == 0 ; j--) {
					population[i].p[j]++;
					currentSum += coinsTypeArray[j]; // Atualiza a soma
					if(currentSum >= data.target){
						stopAdd = 1;
					}
				}
			} else if (currentSum > data.target) {
				// Remove a moeda de MENOR valor presente
				stopRem = 0;
				for (j = 0; stopRem == 0; j++) {
					if (population[i].p[j] > 0) {
						population[i].p[j]--;
						currentSum -= coinsTypeArray[j]; // Atualiza a somayyy
						stopRem = 1;
					}
				}
			}
		}
	}
}
//ALTERAR A FUNCAO ABAIXO
/*
void repair_sofrega(pChromosome pop, struct info data, int mat[][2]) {
	int i,j, object_to_remove;
	float current_w, minimum_v;

	for(i = 0; i < data.popsize; i++) {
		current_w = calculate(pop[i].p,data,mat);
		while(current_w > data.capacity) {
			//Encontra posicao de menor valor
			object_to_remove=-1;
			minimum_v = 100;
			for(j = 0; j < data.numGenes; j++) {
				if(pop[i].p[j] == 1) {
					if(mat[j][i] < minimum_v) {
						minimum_v = mat[j][i];
						object_to_remove = j;
					}
				}
			}
			//Remove objeto
			pop[i].p[object_to_remove] = 0;
			//Recalcula o peso
			current_w = calcula_peso(pop[i].p,data,mat);
		}
	}
}
*/