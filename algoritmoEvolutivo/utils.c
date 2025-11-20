#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"
#include "algorithm.h"
#include <math.h>
void initRandom() {
    srand((unsigned)time(NULL));
}

int random_l_h(int min, int max) {
    return min + rand() % (max-min+1);
}

struct info initData(char *filename, float **coinsTypeArray) {
    struct info x;
    FILE *f;
    int i;

    f = fopen(filename, "rt");
    if (!f) {
        printf("Erro ao abrir o ficheiro %s\n", filename);
        exit(1);
    }

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

    x.populationSize = 100;  // Exemplo
    x.probabilityMutation = 0.1;
    x.probabilityRecombination = 0.5;
    x.tournamentSize = 50;
    x.numGenerations = 1000;
    x.rho = 100;

    fclose(f);
    return x;
}

int flip()
{
    if ((((float)rand()) / RAND_MAX) < 0.5)
        return 0;
    else
        return 1;
}

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



void writeBest(chromosome x, struct info data) {
    int i;

    printf("\nBest individual: %4.1f\n", x.fitness);
    for (i = 0; i < data.numGenes; i++) {
        printf("%d ", x.p[i]);
    }
    putchar('\n');
}



float rand_01()
{
    return ((float)rand())/RAND_MAX;
}