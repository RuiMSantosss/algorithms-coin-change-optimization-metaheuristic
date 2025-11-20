

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"
#include <math.h>

float* initData(char *fileName, float *coinsValueToReach, int *numberCoinsType) {
    FILE *file;
    file = fopen(fileName, "r");
    float *coinsTypeArray, *auxCoinsTypeArray;

    if(!file){
            printf("Erro no acesso ao ficheiro");
            exit(1);
    }

    fscanf(file, "%d %f", numberCoinsType, coinsValueToReach);

    coinsTypeArray = malloc(sizeof(float)*(*numberCoinsType));
    if(!coinsTypeArray)
    {
        printf("Erro na alocacao de memoria\n");
        fclose(file);
        exit(1);
    }

    auxCoinsTypeArray = coinsTypeArray;
    for (int i = 0; i < (*numberCoinsType); i++) {
        if (fscanf(file, "%f", auxCoinsTypeArray++) != 1) {
            printf("Erro ao ler o valor da moeda na posicao %d\n", i);
            free(coinsTypeArray);
            fclose(file);
            exit(1);
        }
    }
    fclose(file);
    return coinsTypeArray;
}

float rand_01() {
    return ((float) rand()) / RAND_MAX;
}

void generateInitialSolution(int *solution, float *coinsTypeArray, int numberCoinsType, float coinsValueToReach) {
    float soma = 0.0;
    int i;

    // Inicializa a solução com zeros
    for (i = 0; i < numberCoinsType; i++) {
        solution[i] = 0;
    }

    // Adiciona moedas aleatoriamente
    while (soma < coinsValueToReach) {
        int indice = random_l_h(0, numberCoinsType - 1); // Escolhe aleatoriamente um tipo de moeda
        float novoValor = soma + coinsTypeArray[indice]; // Calcula a soma se adicionarmos esta moeda

        // Probabilidade de permitir ultrapassar ou manter a soma menor
        float probabilidade = rand_01(); // Gera um número aleatório entre 0 e 1

        if (novoValor > coinsValueToReach && probabilidade > 0.5) {
            break; // Permite parar com a soma abaixo ou igual ao valor-alvo em 50% dos casos
        }

        solution[indice]++; // Incrementa a quantidade da moeda selecionada
        soma = novoValor;   // Atualiza a soma total
    }
}
void writeSolution(int *solution, float *coinsTypeArray, int numberCoinsType, float coinsValueToReach) {
    float difference, sum = 0;

    printf("\nValor-Alvo: %.2f\n", coinsValueToReach);
    printf("\nDetalhes da Solucao:\n");

    for (int i = 0; i < numberCoinsType; i++) {
        printf("Moeda %.2f: %d unidades\n", coinsTypeArray[i], solution[i]);
        sum += solution[i] * coinsTypeArray[i];
    }
    if (fabs(sum - coinsValueToReach) < 0.00001) {
        printf("Solucao valida!\n");
    } else if (sum < coinsValueToReach) {
        difference = coinsValueToReach - sum;
        printf("Solucao invalida! Valor da Solucao -> %.2f  Solucao menor que o desejado. Diferenca de: %f\n",sum, difference);
    } else if (sum > coinsValueToReach) {
        difference = sum - coinsValueToReach;
        printf("Solucao invalida! Valor da Solucao -> %.2f  Solucao maior que o desejado. Diferenca de: %f\n",sum, difference);
    }
}

int random_l_h(int min, int max)
{
    return min + rand() % (max-min+1);
}

// Inicializa o gerador de numeros aleatorios
void initRandom()
{
    srand((unsigned)time(NULL));
}


void copySolution(int *dest, int *src, int numberCoinsType) {
    //copia os valores
    for (int i = 0; i < numberCoinsType; i++) {
        dest[i] = src[i];
    }
}

