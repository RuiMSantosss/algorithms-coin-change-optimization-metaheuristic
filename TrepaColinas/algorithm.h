//
// Created by TomasPaiva on 26/11/2024.
//

#ifndef TRABALHO_2_IIA_ALGORITMO_H
#define TRABALHO_2_IIA_ALGORITMO_H

void substitute(int a[], int b[], int n);

float trepaColinas(int solution[], float *coinsTypeArray, int numberCoinsType, float coinsValueToReach, int numIter, int percentage);

float calculateFit(int solution[], float *coinsTypeArray, int numberCoinsType, float coinsValueToReach);

void generateNeighbour(int solution[], int newSol[], int numberCoinsType, float coinsValueToReach, float *coinsTypeArray);

#endif //TRABALHO_2_IIA_ALGORITMO_H
