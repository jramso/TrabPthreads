#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1
#pragma comment(lib,"pthreadVC2.lib")
#define HAVE_STRUCT_TIMESPEC

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define LINHA 100
#define COLUNA 100
int soma;

typedef struct Matriz{
	int linhas;
	int colunas;
	int *matrizA[];
}Matriz;

//void* threadFunc(void*);
void* calcPrimo(void*);
int **criaMatriz();
int main(int argc, char* argv[]) {
	int **minhamatriz=criaMatriz();

	for (int i = 0; i < LINHA; i++) {
		for (int j = 0; j < COLUNA; j++) {
			minhamatriz[i][j] = rand() % 3199;
			printf(" %d ", minhamatriz[i][j]);
		}
	}
	pthread_t thread;
	int parametro = 0;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_create(&thread, &attr, calcPrimo, &parametro);
	pthread_join(thread, NULL);
	printf("soma= %d ", soma);

	printf("Print do Main");
	return 0;
}
/*void* threadFunc(void* nenhum) {
	printf("Print da Thread\n");
}*/

void* calcPrimo(void* param) {
	int num = *((int*)param);
	int cont = 0;
	for (int i = 1; i <= num; i++) {
		if (num % i == 0) {
			cont++;
		}
	}
	if (cont == 2) {
		printf("\nO numero %d E primo\n\n", num);
	}
	else {
		printf("\nO numero %d NAO e primo\n\n", num);
	}
	pthread_exit(0);
}

int** criaMatriz() {

	int** matriz;
	int i, j;

	//vetor de LINHAS ponteiros
	matriz = malloc(LINHA * sizeof(int*));

	matriz[0] = malloc(LINHA * COLUNA * sizeof(int));


	for (i = 1; i < LINHA; i++) {
		for (j = 0; j < COLUNA; j++) {
			matriz[i] = matriz[0] + i * COLUNA;
		}
	}

	



	//for (i = 0; i < LINHA; i++) {
	//	for (j = 0; j < COLUNA; j++) {
	//		print("%d", matriz[i][j]);
	//	}
	//}

	return matriz;

}


