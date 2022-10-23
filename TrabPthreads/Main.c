#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1
#pragma comment(lib,"pthreadVC2.lib")
#define HAVE_STRUCT_TIMESPEC

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

/* CONSTANTES QUE NAO SAO CONSTANTES */
#define NUM_THREADS 2
#define LINHA 100
#define COLUNA 100
#define MACRO_LINHA 5
#define MACRO_COLUNA 5
#define COLFIL (COLUNA/MACRO_COLUNA)
/* GLOBAL VARS */
int soma;
int** matriz;
int** macro;




/*  REF METODOS */
//int calcPrimo(int n);
void* calcPrimo(void*);

void criaMatriz();
int** macroBloco();

/* MAIN */
int main(int argc, char* argv[]) {
	criaMatriz();

	for (int i = 0; i < LINHA; i++) {
		for (int j = 0; j < COLUNA; j++) {
			printf(" %d ", matriz[i][j]);
		}
		printf("\n");
	}
		printf("\n");
		printf("\n");

	macroBloco(400);
	for (int i = 0; i < MACRO_LINHA; i++) {
		for (int j = 0; j < MACRO_COLUNA; j++) {
			printf(" %d ", macro[i][j]);
		}
		printf("\n");
	}

	
	pthread_t thread[NUM_THREADS];
	/* CRIAÇÃO DAS THREADS */
	for (int k = 0; k < 10; k++) {
		int parametro = matriz[0][k];
		for (int i = 0; i < NUM_THREADS; i++) {
			pthread_create(&thread[i], NULL, calcPrimo, &parametro);
		}

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
//int calcPrimo(int n) {
void* calcPrimo(void* param) {
	int n = *((int*)param);
	int cont = 0;
	for (int i = 2; i <= sqrt(n); i++) {
		if (n % i == 0) {
			cont++;
		}
	}
	if (cont == 0) {
		soma++;
		printf("\nO numero %d E PRIMO\n\n", n);
	}
	else {
		printf("\nO numero %d NAO e primo\n\n", n);
	}
	pthread_exit(0);
}//fim calcPrimo

// 2. metodo que aloca memória e preenche a matriz

void criaMatriz() {

	int i, j;

	// aloca um vetor de LIN ponteiros para linhas
	matriz = malloc(LINHA * sizeof(int*));

	// aloca cada uma das linhas (vetores de COL inteiros)
	for (i = 0; i < LINHA; i++)
		matriz[i] = malloc(COLUNA * sizeof(int));

	//percorre e preenche a matriz
	for (int i = 0; i < LINHA; i++) {
		for (int j = 0; j < COLUNA; j++) {
			//matriz[i][j] = rand() % 32000;
			matriz[i][j] = i;
		}
	}
}//fim criaMatriz()

// 3. metodo que cria as submatrizes "macroblocos"

int** macroBloco(int n) {
	//numero do macrobloco
	int lin, col;
	macro = malloc(MACRO_LINHA * sizeof(int*));
	macro[0] = malloc(MACRO_LINHA * MACRO_COLUNA * sizeof(int));
	//ajusta os ponteiros de linhas onde i>0
	for (lin = 1; lin < MACRO_LINHA; lin++) {
		for (col = 0; col < MACRO_COLUNA; col++) {
			macro[lin] = macro[col] + lin * MACRO_COLUNA;
		}
	}

	//calculo de onde o macrobloco entra na matriz
	for (lin = 0; lin < MACRO_LINHA; lin++) {
		for (col = 0; col < MACRO_COLUNA; col++) {
			//N=0
			macro[lin] = matriz[lin];
			macro[lin][col] = matriz[(lin + (MACRO_LINHA * (n / COLFIL)))][col + (MACRO_COLUNA*n%COLFIL)];

		}
	}

}//fim macroBloco()
