#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1
#pragma comment(lib,"pthreadVC2.lib")
#define HAVE_STRUCT_TIMESPEC

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* CONSTANTES QUE NAO SAO CONSTANTES */
#define NUM_THREADS 1
#define LINHA 100
#define COLUNA 100
#define MACRO_LINHA 5
#define MACRO_COLUNA 5
/* GLOBAL VARS */
int soma;
int** matriz;


/*  REF METODOS */
void* calcPrimo(void*);
int **criaMatriz();

/* MAIN */
int main(int argc, char* argv[]) {
	criaMatriz();

	for (int i = 0; i < LINHA; i++) {
		for (int j = 0; j < COLUNA; j++) {
			printf(" %d ", matriz[i][j]);
		}
	}
	pthread_t thread[NUM_THREADS];
	/* CRIAÇÃO DAS THREADS */
	int parametro = 10;
	for (int i = 0; i < NUM_THREADS; i++) {
		pthread_create(&thread[i], NULL, calcPrimo, &parametro);
	}
	/* JOIN NAS THREADS*/
	for (int j = 0; j < NUM_THREADS; j++) {

		pthread_join(thread[j], NULL);
	}
	printf("soma= %d ", soma);

	printf("Print do Main");
	return 0;
}

/* METODOS */
// 1. metodo que confere se um numero é primo
void* calcPrimo(void* param) {
	int num = *((int*)param);
	int cont = 0;
	for (int i = 1; i <= num; i++) {
		if (num % i == 0) {
			cont++;
		}
	}
	if (cont == 2) {
		soma++;
	}
	else {
		printf("\nO numero %d NAO e primo\n\n", num);
	}
	pthread_exit(0);
}//fim calcPrimo

// 2. metodo que aloca memória e preenche a matriz

int** criaMatriz() {

	int i, j;

	//vetor de LINHAS ponteiros
	matriz = malloc(LINHA * sizeof(int*));
	//vetor de todos os elementos da matriz
	matriz[0] = malloc(LINHA * COLUNA * sizeof(int));
	//ajusta os ponteiros de linhas onde i>0
	for (i = 1; i < LINHA; i++) {
		for (j = 0; j < COLUNA; j++) {
			matriz[i] = matriz[0] + i * COLUNA;
		}
	}
	//percorre e preenche a matriz
	for (int i = 0; i < LINHA; i++) {
		for (int j = 0; j < COLUNA; j++) {
			matriz[i][j] = rand() % 3199;
		}
	}
}//fim criaMatriz()

// 3. metodo que cria as submatrizes "macroblocos"

int macroBloco() {
	int** macro;


}//fim macroBloco()
