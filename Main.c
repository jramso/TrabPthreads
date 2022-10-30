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
#include <semaphore.h>


/* CONSTANTES QUE NAO SAO CONSTANTES */
#define NUM_THREADS 8
#define LINHA 1000
#define COLUNA 1000
#define MACRO_LINHA 10
#define MACRO_COLUNA 10
#define COLFIL (COLUNA/MACRO_COLUNA)
#define COLLIN (LINHA/MACRO_LINHA)
#define TAMANHO ((LINHA * COLUNA) / (MACRO_COLUNA * MACRO_LINHA))

/* GLOBAL VARS */
int soma=0;
int somaSingle = 0;
int** matriz=NULL;
int linha = 0;
int coluna = 0;

int global_index;
struct Macro {
	int read;
	int minLin;
	int maxLin;
	int minCol;
	int maxCol;
}macroBloc[TAMANHO];

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexBloco = PTHREAD_MUTEX_INITIALIZER;



int wc = 0;

/*  REF METODOS */

int ehPrimo(int n);
void criaMatriz();
void macroBloco(int param);
void* primoBloco(void*);
void* primoBlocoSingle(void*);
void LiberaMatriz(int** M, int m);

/* MAIN */
int main(int argc, char* argv[]) {

	criaMatriz();//cria a matriz
	//criando tamanho macroblocos
	for (int i = 0; i < TAMANHO; i++) {
		macroBloco(i);
	}
	
	/* CONTADOR DE TEMPO*/
		LARGE_INTEGER tempoInicial, tempoFinal, freq;
		LARGE_INTEGER tempoInicialSingle, tempoFinalSingle, freqSingle;
		float tempoTotal, tempoTotalSingle;
	/* CRIAÇÃO DAS THREADS */
	/*Single Thread*/
		//pthread_t threadSingle;
		//pthread_create(&threadSingle, NULL, primoBlocoSingle, NULL);
		//pthread_join(threadSingle,NULL);

		QueryPerformanceCounter(&tempoInicialSingle);//inicio do contador
			primoBlocoSingle(1);//Single
		QueryPerformanceFrequency(&freqSingle);
		QueryPerformanceCounter(&tempoFinalSingle);
		tempoTotalSingle= (float)(tempoFinalSingle.QuadPart - tempoInicialSingle.QuadPart) / freqSingle.QuadPart;



	QueryPerformanceCounter(&tempoInicial);//inicio do contador
	/*Multi Thread*/
		pthread_t thread[NUM_THREADS];	//vetor de threads
		for (int i = 0; i < NUM_THREADS; i++) {
			//thread_args[i] = i;
			pthread_create(&thread[i], NULL, primoBloco, &i);
			printf("IN MAIN: numero da thread : %d\n", i);
		}
		printf("-------------\n");
		/* JOIN NAS THREADS*/
		for (int j = 0; j < NUM_THREADS; j++) {
			printf("\nIN MAIN: numero do Join %d\n", j);
			pthread_join(thread[j], NULL);
		}
	/* finaliza o tempo*/
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&tempoFinal);
	tempoTotal = (float)(tempoFinal.QuadPart - tempoInicial.QuadPart) / freq.QuadPart;

	/* Saida de dados*/
	printf("SOMA SINGLE = %d \n", somaSingle);
	printf("Tempo SINGLE = %f \n", tempoTotalSingle);

	printf("SOMA MULTI= %d \n", soma);
	printf("Tempo MULTI= %f \n", tempoTotal);

	/*NULL threads e mutexes*/
	//pthread_mutex_destroy(&mutex);
	//pthread_exit(NULL);
	/* Limpa o heap*/
	LiberaMatriz(matriz, LINHA);

	return 0;
}//FIM_MAIN()


/* METODOS */
// 1. metodo que confere se um numero é primo
int ehPrimo(int n) {
		if (n > 1) {
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
		for (j = 0; j < COLUNA; j++) {
			matriz[i][j] = rand() % 32000;
		}
	//printf("matriz linha? [%d]", matriz[i]);
	}
	printf("MATRIZ COMPLETA\n");

}//fim criaMatriz()

// 3. metodo que separa a matriz em submatrizes por indices
void macroBloco(int n) {
	//numero do macrobloco


	/*LIMITE DE MACROBLOCOS*/assert(n < ((LINHA* COLUNA) / (MACRO_COLUNA * MACRO_LINHA)));
	/*ULTIMOS MACROBLOCOS FORA DO LIMITE DE MEMORIA*/assert((LINHA * COLUNA) % (MACRO_COLUNA * MACRO_LINHA) == 0);

	if (((n % COLFIL) == 0) && (linha < (LINHA)) && n > 0)
		linha+=MACRO_LINHA;
	if (n > 0) 
		coluna = MACRO_COLUNA*(n%COLFIL);


	macroBloc[n].minLin = linha;
	macroBloc[n].minCol = coluna;
	macroBloc[n].maxCol = coluna + MACRO_COLUNA;
	macroBloc[n].maxLin = linha + MACRO_LINHA;
	macroBloc[n].read = 1;
}//fim macroBloco()


// 4. Metodo executado pelas threads
void* primoBloco(void* numblock) {

	int x = *((int*)numblock);
	int j;

	int local_index=0;
	while (local_index < (TAMANHO) && global_index<TAMANHO) {
		int partial_sum = 0;
		pthread_mutex_lock(&mutex);
		local_index = global_index;
		global_index++;
		pthread_mutex_unlock(&mutex);
		if (macroBloc[local_index].read == 1) {
			macroBloc[local_index].read = 0;
			for (int lin = macroBloc[local_index].minLin; lin < macroBloc[local_index].maxLin; lin++) {
				for (int col = macroBloc[local_index].minCol; col < macroBloc[local_index].maxCol; col++) {
					if (ehPrimo(matriz[lin][col])==1) {
						partial_sum++;
					}
				}
			}
			pthread_mutex_lock(&mutexBloco);
			soma+=partial_sum;
			pthread_mutex_unlock(&mutexBloco);
		}
	}

	
			

}
// 5. Metodo Executado Pela Single thread
void* primoBlocoSingle(void* numblock) {

	for (int lin = 0; lin < LINHA; lin++) {
		for (int col = 0; col < COLUNA; col++) {
			if (ehPrimo(matriz[lin][col])==1)
				somaSingle++;
		}
	}			
}//fim primoBlocoSingle()

// 6. Metodo que libera a Matriz
void LiberaMatriz(int** M, int m) {
	int i;
	for (i = 0; i < m; i++)
		free(M[i]);
	free(M);
}//fim LiberaMatriz

