#include <stdio.h>
#include <stdlib.h>
#include "algorithm.h"
#include "utils.h"
#include <math.h>

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

void tournamentWithNSize(pChromosome population, struct info data, pChromosome parents) {
    int i, newX, counter, toDiscard;
    int tournamentSize = data.tournamentSize;
    int x[tournamentSize];
    float thisFitness, bestFitness;

    // Realiza populationSize torneios
    for (i = 0; i < data.populationSize; i++) {
        counter = 1; // Inicia o contador para o torneio
        toDiscard = 0;

        // Seleciona o primeiro elemento aleatório para o torneio
        x[0] = random_l_h(0, data.populationSize - 1);
        bestFitness = population[x[0]].fitness;
        parents[i] = population[x[0]]; // Inicializa o pai com o primeiro indivíduo

        while (counter < tournamentSize) {
            toDiscard = 0;
            newX = random_l_h(0, data.populationSize - 1);

            // Verifica se o ponto novo já está no torneio
            for (int j = 0; j < counter; j++) {
                if (x[j] == newX) {
                    toDiscard = 1;
                    break; // Sai do loop se já existir
                }
            }

            // Se o ponto é válido, adiciona ao torneio
            if (!toDiscard) {
                x[counter] = newX;
                thisFitness = population[newX].fitness;

                // Atualiza o melhor fitness
                if (thisFitness < bestFitness) {
                    parents[i] = population[newX];
                    bestFitness = thisFitness;
                }
                counter++;
            }
        }
    }
}


void geneticOperators(pChromosome parents, struct info data, pChromosome offspring) {
    // Recombinação com um ponto de corte
    //crossover(parents, data, offspring);
    // Recombinacao com dois pontes de corte
    crossoverTwoPoints(parents, data, offspring);

    // Mutação binária
    //mutation(offspring, data);
    // Mutação de troca
    mutationSwap(offspring, data);
}

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


void crossoverTwoPoints(pChromosome parents, struct info d, pChromosome offspring) {
    int i, j, point1, point2, dummy;

    for (i = 0; i < d.populationSize; i+=2) // percorre a populao de pais de 2 em 2 (sao 2 pais)
    {
        if (rand_01() < d.probabilityRecombination) // pr é a probabilidade de recombinação
        {
            point1 = random_l_h(0, d.numGenes-1);
            point2 = random_l_h(0, d.numGenes-1);
            while(point2 == point1) { // para os 2 pontos serem diferentes
                point2 = random_l_h(0, d.numGenes-1);
            }

            if(point2 < point1) { //ordenar
                dummy = point1;
                point1 = point2;
                point2 = dummy;
            }
            // Inicializa os filhos iguais aos pais
            offspring[i] = parents[i];
            offspring[i+1] = parents[i+1];

            // Altera a parte central
            for (j = point1; j < point2; j++) //troca
            {
                offspring[i].p[j]= parents[i+1].p[j];
                offspring[i+1].p[j] = parents[i].p[j];
            }
        }
        else
        {
            offspring[i] = parents[i];
            offspring[i+1] = parents[i+1];
        }
    }
}

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
            //offspring[i].p[obj1]--;  // Remove uma moeda de obj1
            //offspring[i].p[obj2]++;  // Adiciona uma moeda a obj2
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
    int i, j;
    float currentSum;

    for (i = 0; i < data.populationSize; i++) {
        currentSum = calculateSum(population[i].p, data, coinsTypeArray);
        // printf("\n===========FEZ BEM========== %f %d\n", currentSum, i);
        // Ajusta a solução enquanto a soma não estiver próxima do valor alvo
        while (fabs(currentSum - data.target) > 0.001) {
            if (currentSum < data.target) {
                // Adiciona a moeda de MAIOR valor que possa ser usada
                for (j = data.numGenes - 1; j >= 0 ; j--) {
                    if (fabs(currentSum - data.target) < j) {
                        continue;
                    }
                    population[i].p[j]++;
                    currentSum += coinsTypeArray[j]; // Atualiza a soma
                    if(currentSum >= data.target){
                        break;
                    }
                }
            } else if (currentSum > data.target) {
                // Remove a moeda de MENOR valor presente
                for (j = 0; j < data.numGenes; j++) {
                    if (population[i].p[j] > 0) {
                        population[i].p[j]--;
                        currentSum -= coinsTypeArray[j]; // Atualiza a somayyy
                        break;
                    }
                }
            }
        }
    }
}