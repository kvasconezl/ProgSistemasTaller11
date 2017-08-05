#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

typedef struct estructuraHilo {
    int inicio;
    int cantidad;
    int *arreglo;
} structH;

int aleatorio(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}

double tiempo_actual() {
    struct timespec tsp;
    clock_gettime(CLOCK_REALTIME, &tsp);
    double secs = (double) tsp.tv_sec;
    double nano = (double) tsp.tv_nsec / 1000000000.0;
    return secs + nano;
}

void array_num(int *arreglo, int n_arreglos) {
    int i = 0;
    for (i = 0; i < n_arreglos; i++) {
        arreglo[i] = aleatorio(0, 255);
    }
}

void *sumar(void *arg) {
    structH *aegumento = (structH *) arg;
    int i = 0;
    long suma = 0;
    int maximo = aegumento->cantidad + aegumento->inicio;
    for (i = aegumento->inicio; i < maximo; i++) {
        suma = suma + (aegumento->arreglo)[i];
    }
    return (void *) suma;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("USO: ./hilo <TAMAÑO> <NUMERO_HILOS>\n");
        exit(-1);
    }

    double ti = tiempo_actual();
    int n_elem = atoi(argv[1]);
    int n_hilos = atoi(argv[2]);
    int tamano;
    int resto = 0;
    void *retorno;
    srand(time(0));
    int *arreglos = malloc(n_elem * sizeof(int));
    memset(arreglos, 0, n_elem * sizeof(int));
    long suma = 0;

    pthread_t *idArr = malloc(n_hilos * sizeof(pthread_t));
    int status, i, inicio = 0;
    array_num(arreglos, n_elem);

    if (n_elem % n_hilos != 0) {
        tamano = n_elem / n_hilos;
        resto = n_elem % n_hilos;
    }
    tamano = n_elem / n_hilos;
    for (i = 0; i < n_hilos; i++) {
        structH *estructuraH = malloc(sizeof(structH));
        estructuraH->inicio = inicio;
        if (i == (n_hilos - 1)) {
            estructuraH->cantidad = tamano + resto;
        } else {
            estructuraH->cantidad = tamano;
        }
        estructuraH->arreglo = arreglos;

        status = pthread_create(&idArr[i], NULL, sumar, (void *) estructuraH);
        if (status < 0) {
            fprintf(stderr, "Error al crear el hilo : %d\n", i);
            exit(-1);
        }
        inicio += tamano;
    }

    printf("Hilo principal en espera...\n");
    for (i = 0; i < n_hilos; i++) {
        retorno = 0;
        int status1 = pthread_join(idArr[i], &retorno);
        if (status1 < 0) {
            fprintf(stderr, "Error al esperar por el hilo.\n");
            exit(-1);
        }
        printf("Suma parcial hilo %d: %ld\n", i, (long) retorno);
        suma = suma + (long) retorno;
    }
    printf("Suma total: %ld\n", suma);
    double tf = tiempo_actual() - ti;
    printf("Tiempo: %f \n", tf);
    return 0;
}
