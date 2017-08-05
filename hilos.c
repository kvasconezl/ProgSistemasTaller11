#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

typedef struct mi_estructuraHilo {
    int *arreglo;
    int inicio;
    int final;
} estructuraH;

int aleatorio(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}

int *generaArreglo(int size) {
    int *arreglo;
    arreglo = (int *) malloc(size * sizeof(int));
    int i;
    srand(time(0));

    for (i = 0; i < size; i++) {
        int valor = aleatorio(0, 200);
        arreglo[i] = valor;
    }

    return arreglo;
}

double obtenerTiempoActual() {
    struct timespec tsp;

    clock_gettime(CLOCK_REALTIME, &tsp);

    double secs = (double) tsp.tv_sec;
    double nano = (double) tsp.tv_nsec / 1000000000.0;

    return secs + nano;
}

int *funcion_hilo_suma(void *arg) {
    estructuraH *argumentos = (estructuraH *) arg;
    int i;
    long suma = 0;

    for (i = argumentos->inicio; i < argumentos->final; i++) {
        // printf("%d, %d, %d\n", (argumentos->arreglo)[i], argumentos->inicio, argumentos->final);
        suma = suma + (long) (argumentos->arreglo)[i];
        // (argumentos->arreglo)[i] = 0;
    }

    return (int *) suma;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("USO: ./hilo <TAMAÑO> <NUMERO_HILOS>\n");
        exit(-1);
    }

    int tamanio = atoi(argv[1]);
    int hilos = atoi(argv[2]);
    int espacio = tamanio / hilos;
    srand(time(0));
    printf("Tamaño arreglo: %d\n", tamanio);
    printf("Numero de hilos: %d\n", hilos);
    printf("Tamaño recorrer por cada hilo: %d\n", espacio);
    pthread_t id;

    int *arreglo = (int *) malloc(tamanio * sizeof(int));
    pthread_t idArr[hilos];
    // void *sumaRetorno = (void *) malloc(hilos * sizeof(NULL));
    void *sumas[hilos];
    // sumas = (long *) malloc(hilos * sizeof(long));

    for (int i = 0; i < tamanio; i++) {
        arreglo[i] = aleatorio(1, 200);
        printf("%d ", arreglo[i]);
    }
    printf("\n");

    int i;
    long suma = 0;
    int inicio = 0;

    estructuraH *estructuraHilos = malloc(sizeof(estructuraH));
    estructuraHilos->arreglo = arreglo;

    for (i = 0; i < hilos; i++) {
        estructuraHilos->inicio = inicio;
        estructuraHilos->final = inicio + espacio;

        int status;

        status = pthread_create(&id, NULL, (void *) funcion_hilo_suma, estructuraHilos);

        if (status != 0) {
            fprintf(stderr, "Error al crear el hilo 1\n");
            exit(-1);
        }

        idArr[i] = id;
        inicio = inicio + espacio;

        //     void *sumaRetorno = NULL;

        //     int status1 = pthread_join(id, &sumaRetorno);
        //     suma = suma + (long) sumaRetorno;

        //     if (status1 < 0) {
        //         fprintf(stderr, "Error al esperar por el hilo 1\n");
        //         exit(-1);
        //     }

        //     printf("Suma: %ld\n", suma);

        //     return 0;
        // }
    }

    // long sumaRetorno[hilos];

    for (i = 0; i < hilos; i++) {
        // void *sumaRetorno = NULL;
        sumas[i] = (void *) malloc(sizeof(int *));
        int status1 = pthread_join(idArr[i], &sumas[i]);

        if (status1 != 0) {
            fprintf(stderr, "Error al esperar por el hilo 1\n");
            exit(-1);
        }
        // suma = suma + (long) sumas[i];
        // printf("Suma %d: %ld\n", i, suma);
    }

    for (i = 0; i < hilos; i++) {
        suma = suma + (long) sumas[i];
        printf("Suma %d: %ld\n", i, suma);
        printf("Suma parcial: %d\n", (int) sumas[i]);
    }
    return 0;
}
