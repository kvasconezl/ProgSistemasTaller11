#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <unistd.h>

int aleatorio(int min, int max){
	return (rand() % (max-min+1)) + min;
	
}

double obtenerTiempoActual(){
	struct timespec tsp;

	clock_gettime( CLOCK_REALTIME, &tsp);

	double secs = (double)tsp.tv_sec;
	double nano = (double)tsp.tv_nsec / 1000000000.0;

	return secs + nano;
}

int main(){
	srand(time(0));
	printf("Tiempo primero %.9f\n", obtenerTiempoActual());
	printf("Aleatorio %d\n", aleatorio(1,200));
	printf("Tiempo segundo %.9f\n", obtenerTiempoActual());
	
	
	return 1;

}


