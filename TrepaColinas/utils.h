
#ifndef TRABALHO_2_IIA_UTILS_H
#define TRABALHO_2_IIA_UTILS_H


float* initData(char *fileName, float *coinsValueToReach, int *numberCoinsType);

void generateInitialSolution(int *solution, float *coinsTypeArray, int numberCoinsType, float coinsValueToReach);

int random_l_h(int min, int max);

void writeSolution(int *solution, float *coinsTypeArray, int numberCoinsType, float coinsValueToReach);

void initRandom();

void copySolution(int *dest, int *src, int numberCoinsType);

#endif //TRABALHO_2_IIA_UTILS_H
