#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>

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

int main(){
	srand(time(0));
	printf("Tiempo primero %.9f\n", obtenerTiempoActual());
	printf("Aleatorio %d\n", aleatorio(7,200));
	printf("Tiempo segundo %.9f\n", obtenerTiempoActual());
	
	int *a=generaArreglo(5);

	for (int i = 0; i < 5; i++)
	{
		printf("%d\n",a[i] );
	}

	
	
	return 1;

}


