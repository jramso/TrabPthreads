#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1
#pragma comment(lib,"pthreadVC2.lib")
#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 10

/*---GLOBAL VARIABLES*/
int sum = 0;


void* threadFunc(void*);
int main(int argc, char* argv[]) {
	/* codigo do professor*/
	pthread_t thread;
	if (pthread_create(&thread, NULL, threadFunc, NULL) != 0) {
		perror("Pthread_create falhou!");
		exit(1);
	}
	if (pthread_join(thread, NULL) != 0) {
		perror("Pthread_join falhou!");
		exit(1);
	}

	printf("Print do Main");
	/*FIM_codigo do professor*/
	return 0;
}
void* threadFunc(void* nenhum) {
	printf("Print da Thread\n");
	printf("Numero da Thread: %d \n",pthread_self());
}

void* calcPrimo(void* num) {
	
	int numero = *((int*)num);
}

