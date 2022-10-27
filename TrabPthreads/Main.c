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
#include <math.h>
#include <assert.h>
#include <Windows.h>


/* CONSTANTES QUE NAO SAO CONSTANTES */
#define NUM_THREADS 8
#define LINHA 10000
#define COLUNA 10000
#define MACRO_LINHA 100
#define MACRO_COLUNA 100
#define COLFIL (COLUNA/MACRO_COLUNA)
#define TAMANHO ((LINHA * COLUNA) / (MACRO_COLUNA * MACRO_LINHA))

/* GLOBAL VARS */
int soma=0;
int** matriz;
int linha = 0;
struct Macro {
	int** matrizmacro;
}macroBloc[TAMANHO];
pthread_mutex_t mutex;

/*  REF METODOS */
int ehPrimo(int n);
void criaMatriz();
void macroBloco(int param);
void* primoBloco(void*);
void LiberaMatriz(int** M, int m);

/* MAIN */
int main(int argc, char* argv[]) {

	criaMatriz();//cria a matriz
	pthread_mutex_init(&mutex, NULL);//inicia mutex

	//criando TAMANHO macroblocos
	for (int i = 0; i < TAMANHO; i++) {
		macroBloco(i);
	}

	/* CONTADOR DE TEMPO*/
		LARGE_INTEGER tempoInicial, tempoFinal, freq;
		float tempoTotal;
	/* CRIAÇÃO DAS THREADS */
	//vetor de threads
	pthread_t thread[NUM_THREADS];
		QueryPerformanceCounter(&tempoInicial);//inicio do contador

	for (int i = 0; i < NUM_THREADS; i++) {
		pthread_create(&thread[i], NULL, primoBloco, &i);
		printf("IN MAIN: numero da thread : %d\n", i);
	}
	printf("-------------\n");
	/* JOIN NAS THREADS*/
	for (int j = 0; j < NUM_THREADS; j++) {
		printf("\nIN MAIN: numero do Join %d\n", j);
		pthread_join(thread[j], NULL);
	}

	QueryPerformanceCounter(&tempoFinal);
	QueryPerformanceFrequency(&freq);
	tempoTotal = (float)(tempoFinal.QuadPart - tempoInicial.QuadPart) / freq.QuadPart;

	/* Saida de dados*/
	printf("SOMA = %d \n", soma);
	printf("Tempo = %f \n", tempoTotal);

	/*NULL threads e mutexes*/
	pthread_mutex_destroy(&mutex);
	pthread_exit(NULL);

	LiberaMatriz(matriz, LINHA);

	return 0;
}//FIM_MAIN()


/* METODOS */
// 1. metodo que confere se um numero é primo
int ehPrimo(int n) {
		int cont = 0;
		for (int i = 2; i <= sqrt(n); i++) {
			if (n % i == 0) {
				cont++;
			}
		}
		if (cont == 0) {
			return 1;
		}
		else {
			return 0;
		}
		//pthread_exit(0);
}//fim calcPrimo

// 2. metodo que aloca memória e preenche a matriz
void criaMatriz() {

	int i, j;

	// aloca um vetor de LINHA ponteiros para linhas
	matriz = malloc(LINHA * sizeof(int*));

	// aloca cada uma das linhas (vetores de COL inteiros)
	for (i = 0; i < LINHA; i++)
		matriz[i] = malloc(COLUNA * sizeof(int));

	//percorre e preenche a matriz
	for (int i = 0; i < LINHA; i++) {
		for (int j = 0; j < COLUNA; j++) {
			matriz[i][j] = rand() % 32000;
			//matriz[i][j] = j;
		}
	}
}//fim criaMatriz()

// 3. metodo que cria as submatrizes macroblocos
void macroBloco(int n) {
	//numero do macrobloco
	int** macro;
	int lin, col;

	// aloca um vetor de LIN ponteiros para linhas
	macro = malloc(MACRO_LINHA * sizeof(int*));

	// aloca cada uma das linhas (vetores de COL inteiros)
	for (lin = 0; lin < MACRO_LINHA; lin++)
		macro[lin] = malloc(MACRO_COLUNA * sizeof(int));

	/*LIMITE DE MACROBLOCOS*/assert(n < ((LINHA * COLUNA) / (MACRO_COLUNA * MACRO_LINHA)));
	/*ULTIMOS MACROBLOCOS FORA DO LIMITE DE MEMORIA*/assert((LINHA * COLUNA) % (MACRO_COLUNA * MACRO_LINHA) == 0);

	// quando chegar no ultimo macrobloco da fileira de colunas quebrar fileira de linhas
	if (((n % COLFIL) == 0)&&(linha<(LINHA/MACRO_LINHA))&&n>0) {
			linha++;
	}
	for (lin = 0; lin < MACRO_LINHA; lin++) {
		for (col = 0; col < MACRO_COLUNA; col++) {
			macro[lin][col] = matriz[(lin + (linha*MACRO_LINHA))][col + (MACRO_COLUNA*(n%COLFIL))];

		}
	}
	macroBloc[n].matrizmacro = macro;
}//fim macroBloco()

// 4. Metodo executado pelas threads
void* primoBloco(void* numblock) {
	int x = *((int*)numblock);
	for (int thread = x; (thread+NUM_THREADS)<(TAMANHO+(NUM_THREADS)-1);thread+=NUM_THREADS) {
		//printf("\nEntrou Thread [%d] \n", x);
		for (int i = 0; i < MACRO_LINHA; i++) {

			for (int j = 0; j < MACRO_COLUNA; j++) {
				if (ehPrimo(macroBloc[thread].matrizmacro[i][j])==1){

					pthread_mutex_lock(&mutex);
						soma++;
					pthread_mutex_unlock(&mutex);
				}
			}
		}
		//printf("\nMacrobloco [%d]\n", thread);
	}
	//printf("Saiu [%d]\n",x);
}//fim primoBloco()

// 5. Metodo que libera a Matriz
void LiberaMatriz(int** M, int m) {
	int i;
	for (i = 0; i < m; i++)
		free(M[i]);
	free(M);
}//fim LiberaMatriz