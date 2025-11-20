#include <stdio.h>
#include <stdlib.h>
#include "algorithm.h"
#include "utils.h"
#include <math.h>

// Calcula a qualidade de uma solução
// Recebe:  A solução, a, a matriz de adjacências, mat, e o número de vértices, vert
// Devolve: O custo que é o número de ligações que existem na solução
void substitute(int a[], int b[], int n) {
    int i;
    for(i=0; i<n; i++) {
        a[i] = b[i];
    }
}

float calculateFit(int solution[], float *coinsTypeArray, int numberCoinsType, float coinsValueToReach) {
    float sum = 0;
    float totalCoins = 0;

    for (int i = 0; i < numberCoinsType; i++) {
        sum += solution[i] * coinsTypeArray[i];
        totalCoins += solution[i];
    }

    // Penalização para soluções inválidas
    float difference = fabs(sum - coinsValueToReach);
    if (difference > 0.001) {
        return totalCoins + difference * difference * 100; // Penalização quadrática
    }

    return totalCoins; // Retorna o número total de moedas usadas
}

void generateNeighbour(int solution[], int newSol[], int numberCoinsType, float coinsValueToReach, float *coinsTypeArray) {
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


float trepaColinas(int solution[], float *coinsTypeArray, int numberCoinsType, float coinsValueToReach, int numIter, int percentage) {
    int *newSol, i;
    float cost, costNeighbour;

    newSol = malloc(sizeof(int) * numberCoinsType);
    if (newSol == NULL) {
        printf("Erro na alocacao de memoria");
        exit(1);
    }

    // Avalia solução inicial
    cost = calculateFit(solution, coinsTypeArray, numberCoinsType, coinsValueToReach);
    for (i = 0; i < numIter; i++) {
        generateNeighbour(solution, newSol, numberCoinsType, coinsValueToReach, coinsTypeArray);

        costNeighbour = calculateFit(newSol, coinsTypeArray, numberCoinsType, coinsValueToReach);

        // Aceita vizinho se o custo diminuir (problema de minimização)
        int random_percentage = random_l_h(0, 100);
        if (costNeighbour <= cost || random_percentage < percentage) {
            // printf("yupppi! %i - %i %%\n", costNeighbour, random_percentage);
            substitute(solution, newSol, numberCoinsType);
            cost = costNeighbour;
        }
    }

    free(newSol);
    return cost;
}
