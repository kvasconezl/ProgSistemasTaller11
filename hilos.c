#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>

typedef struct mi_estructuraHilo{
	int * arreglo;
	int inicio;
	int final;
} estructuraH;



int aleatorio(int min, int max){
	return (rand() % (max-min+1)) + min;
}

int *generaArreglo(int size){
	int *arreglo;
	arreglo = (int*) malloc(size*sizeof(int));
	int i;
	srand(time(0));
	
	for (i = 0; i < size; i++)
	{
		int valor=aleatorio(0,200);
		arreglo[i] = valor;
	}

	return arreglo;
	
}

double obtenerTiempoActual(){
	struct timespec tsp;

	clock_gettime(CLOCK_REALTIME, &tsp);

	double secs = (double)tsp.tv_sec;
	double nano = (double)tsp.tv_nsec / 1000000000.0;

	return secs + nano;
}


void * funcion_hilo_suma(void *arg){
	
	estructuraH *argumentos = (estructuraH *)arg;	
	int i;
	long suma = 0;

	for(i = argumentos->inicio; i < argumentos->final; i++){
		suma = suma + (argumentos->arreglo)[i];
	}
	
	return (void *)suma;

}



int main(int argc, char ** argv){
	
	int tamanio = atoi(argv[1]);
	int hilos = atoi(argv[2]);
	int espacio = tamanio / hilos;
	srand(time(0));
	printf("Tamaño arreglo: %d\n", tamanio);
	printf("Numero de hilos: %d\n", hilos);
	printf("Tamaño recorrer por cada hilo: %d\n", espacio);
	pthread_t id1;
	
	int *arreglo = (int *)malloc(tamanio*sizeof(int));

	for (int i = 0; i < tamanio; i++){
		arreglo[i] = aleatorio(1, 200);
		printf("%d ", arreglo[i]);
	}
	printf("\n");
	int i;
	long suma = 0;
	int inicio = 0;

	estructuraH * estructuraHilos = malloc(sizeof(estructuraH));
	estructuraHilos->arreglo= arreglo;

	for(i = 0; i < hilos; i++){
	
		estructuraHilos->inicio = inicio;
		estructuraHilos->final = inicio + espacio;

		int status;

		status = pthread_create(&id1, NULL, funcion_hilo_suma, (void *)estructuraHilos);

		if(status < 0){
			fprintf(stderr, "Error al crear el hilo 1\n");
			exit(-1);	
		}
		

		void * sumaRetorno = NULL;

		int status1 = pthread_join(id1, &sumaRetorno);
		suma = suma + (long)sumaRetorno;

		if(status1 < 0){
			fprintf(stderr, "Error al esperar por el hilo 1\n");
			exit(-1);
		}
		inicio = inicio + espacio;
		printf("Suma: %ld\n", suma);
	}
}


