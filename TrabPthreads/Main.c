/* Boa pergunta*/
#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1
#pragma comment(lib,"pthreadVC2.lib")
#define HAVE_STRUCT_TIMESPEC

/*Includes*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <assert.h>


/* CONSTANTES QUE NAO SAO CONSTANTES */
#define NUM_THREADS 8
#define LINHA 10000
#define COLUNA 10000
#define MACRO_LINHA 10
#define MACRO_COLUNA 10
#define COLFIL (COLUNA/MACRO_COLUNA)
#define TAMANHO ((LINHA * COLUNA) / (MACRO_COLUNA * MACRO_LINHA))

/* GLOBAL VARS */
int soma=0;
int** matriz;
int linha = 0;

struct Macro {
	int read;
	int** matrizmacro;
}macroBloc[TAMANHO];

pthread_mutex_t mutex;
pthread_mutex_t mutexLivre;


/*  REF METODOS */

//int calcPrimo(int n);
//void* calcPrimo(void*);

void criaMatriz();
//int** macroBloco(int param);
void macroBloco(int param);

void* primoBloco(void*);

/* MAIN */
int main(int argc, char* argv[]) {

	criaMatriz();
	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_init(&mutexLivre, NULL);

	//for (int i = 0; i < LINHA; i++) {
	//	for (int j = 0; j < COLUNA; j++) {
	//		printf(" %d ", matriz[i][j]);
	//	}
	//	printf("\n");
	//}

	//criando TAMANHO macroblocos
	for (int i = 0; i < TAMANHO; i++) {
		macroBloco(i);
	}



	//printf("\n\nPrint em um Macrobloco C:\n\n");
	//for (int i = 0; i < MACRO_LINHA; i++) {
	//	for (int j = 0; j < MACRO_COLUNA; j++) {
	//		printf(" %d ", macroBloc[49].matrizmacro[i][j]);
	//	}
	//	printf("\n");
	//}

	//vetor de threads
	pthread_t thread[NUM_THREADS];


	/* CRIA��O DAS THREADS */
	double time_spent = 0.0;

	clock_t begin = clock();
	//for (int k = 0; k < (TAMANHO/NUM_THREADS)+1; k++) {
		//int parametro = matriz[0][k];
		for (int i = 0; i < NUM_THREADS; i++) {
			pthread_create(&thread[i], NULL, primoBloco, &i);
			printf("IN MAIN: numero da thread : %d\n", i);
			printf("IN MAIN: numero do macrobloco: %d\n", i);
		}
		printf("-------------\n");
		/* JOIN NAS THREADS*/
		for (int j = 0; j < NUM_THREADS; j++) {
			printf("\nIN MAIN: numero do Join %d\n", j);
			pthread_join(thread[j], NULL);
		}
	//}

	clock_t end = clock();
	time_spent += (double)(end - begin) / CLOCKS_PER_SEC;


	printf("SOMA = %d \n", soma);
	printf("Tempo = %f \n", time_spent);

	printf("Print do Main");
	pthread_mutex_destroy(&mutex);
	pthread_exit(NULL);

	return 0;
}


/* METODOS */
// 1. metodo que confere se um numero � primo
int calcPrimo(int n) {
	//int n = *((int*)param);

		int cont = 0;
		for (int i = 2; i <= sqrt(n); i++) {
			if (n % i == 0) {
				cont++;
			}
		}
		if (cont == 0) {
		//pthread_mutex_lock(&mutex);
			//soma++;
		//pthread_mutex_unlock(&mutex);
			return 1;
			//printf("\nO numero %d E PRIMO\n\n", n);
		}
		else {
			//printf("\nO numero %d NAO e primo\n\n", n);
			return 0;
		}
		//pthread_exit(0);
}//fim calcPrimo

// 2. metodo que aloca mem�ria e preenche a matriz
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
			matriz[i][j] = rand() % 32000;
		}
	}
}//fim criaMatriz()

// 3. metodo que cria as submatrizes "macroblocos"
void macroBloco(int n) {
	//numero do macrobloco
	int num = (int*)n;
	int** macro;
	int lin, col;

	// aloca um vetor de LIN ponteiros para linhas
	macro = malloc(MACRO_LINHA * sizeof(int*));

	// aloca cada uma das linhas (vetores de COL inteiros)
	for (lin = 0; lin < MACRO_LINHA; lin++)
		macro[lin] = malloc(MACRO_COLUNA * sizeof(int));

	/*LIMITE DE MACROBLOCOS*/assert(n < ((LINHA * COLUNA) / (MACRO_COLUNA * MACRO_LINHA)));
	/*ULTIMOS MACROBLOCOS FORA DO LIMITE DE MEMORIA*/assert((LINHA * COLUNA) % (MACRO_COLUNA * MACRO_LINHA) == 0);

	//calculo de onde o macrobloco entra na matriz
	if (((n % COLFIL) == 0)&&(linha<(LINHA/MACRO_LINHA))&&n>0) {
			linha++;
	}
	for (lin = 0; lin < MACRO_LINHA; lin++) {
		for (col = 0; col < MACRO_COLUNA; col++) {
			//N=0
			//macro[lin] = matriz[lin+(linha*MACRO_LINHA)];
			macro[lin][col] = matriz[(lin + (linha*MACRO_LINHA))][col + (MACRO_COLUNA*(n%COLFIL))];

		}
	}
	macroBloc[n].matrizmacro = macro;
	macroBloc[n].read = 0;
	return macro;
}//fim macroBloco()

// 4. Metodo executado pelas threads

void* primoBloco(void* numblock) {
	int qtdPrimo = 0;
	int x = *((int*)numblock);
	for (int thread = x; (thread+NUM_THREADS)<(TAMANHO+(NUM_THREADS));thread+=NUM_THREADS) {
		//printf("\nEntrou Thread [%d] \n", x);
		for (int i = 0; i < MACRO_LINHA; i++) {

			for (int j = 0; j < MACRO_COLUNA; j++) {
				pthread_mutex_lock(&mutex);
				if (calcPrimo(macroBloc[thread].matrizmacro[i][j])==1){

					soma++;
				}
				pthread_mutex_unlock(&mutex);
			}
		}
		//printf("\nMacrobloco [%d]\n", thread);
		macroBloc[x].read = 1;
	}
	//printf("Saiu [%d]\n",x);
	pthread_exit(0);
}