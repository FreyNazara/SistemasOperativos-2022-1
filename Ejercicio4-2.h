#ifndef Ejercicio4_2
#define Ejercicio4_2

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void toString(int** mat, int filas, int cols);

void* procutoPunto(void* mat);

void multiplicaMatrices(int** A, int** B, int fA, int fB, int cA, int cB);

#endif