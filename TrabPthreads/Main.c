#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1
#pragma comment(lib,"pthreadVC2.lib")
#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
int soma;

void* threadFunc(void*);
void* calcPrimo(void*);
int main(int argc, char* argv[]) {
	pthread_t thread;
	int parametro = 11;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_create(&thread, &attr, calcPrimo, &parametro);
	pthread_join(thread, NULL);
	printf("soma= %d ", soma);
	//if (pthread_create(&thread, NULL, threadFunc, NULL) != 0) {
	//	perror("Pthread_create falhou!");
	//	exit(1);
	//}
	//if (pthread_join(thread, NULL) != 0) {
	//	perror("Pthread_join falhou!");
	//	exit(1);
	//}

	printf("Print do Main");
	return 0;
}
void* threadFunc(void* nenhum) {
	printf("Print da Thread\n");
}

void* calcPrimo(void* param) {
	int num = *((int*)param);
	for (int i = 1; i <= num; i++) {
		if (num % i == 0) {
			soma++;
		}
	}
	if (soma == 2) {
		printf("\nO numero %d E primo\n\n", num);
	}
	else {
		printf("\nO numero %d NAO e primo\n\n", num);
	}
	pthread_exit(0);
}


