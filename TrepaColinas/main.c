#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "algorithm.h"

int main() {
    char fileName[100];
    int runs, i;
    float cost, bestCost;
    float coinsValueToReach, MBF = 0;
    int numberCoinsType;
    float *coinsTypeArray;
    int *solution, *bestSolution;

    do {
        printf("Numero de repeticoes: ");
        scanf("%d", &runs);
    } while(runs <= 0);

    printf("\nNome do Ficheiro: ");
    scanf("%s", fileName);

    initRandom();
    coinsTypeArray = initData(fileName, &coinsValueToReach, &numberCoinsType);
    solution = malloc(sizeof(int) * numberCoinsType);
    bestSolution = malloc(sizeof(int) * numberCoinsType);

    if(solution == NULL || bestSolution == NULL)
    {
        printf("Erro na alocacao de memoria");
        exit(1);
    }

    for (i = 0; i < runs; i++) {
        printf("\n========================================================\n");
        generateInitialSolution(solution, coinsTypeArray, numberCoinsType, coinsValueToReach);

        printf("\n========== Solucao Encontrada ==========\n");
        writeSolution(solution, coinsTypeArray, numberCoinsType, coinsValueToReach);

        // Executa o Trepa-Colinxas (Pesquisa Local)
        cost = trepaColinas(solution, coinsTypeArray, numberCoinsType, coinsValueToReach, 100, 1);

        printf("\n====== Repeticao %d ======\n", i + 1);
        writeSolution(solution, coinsTypeArray, numberCoinsType, coinsValueToReach);
        printf("Custo final: %f\n", cost);
        MBF += cost;
        // Atualiza a melhor solução se necessário
        if ((i == 0 || bestCost > cost) && cost > 0.01) {
            bestCost = cost;
            copySolution(bestSolution, solution, numberCoinsType);

        }
    }
    printf("\n========================================================\n");
    printf("\n\nMBF: %f\n", MBF/i);
    printf("\nMelhor solucao encontrada");
    writeSolution(bestSolution, coinsTypeArray, numberCoinsType, coinsValueToReach);
    printf("Custo final: %.2f\n", bestCost);
    free(coinsTypeArray);
    free(solution);
    free(bestSolution);
    return 0;
}