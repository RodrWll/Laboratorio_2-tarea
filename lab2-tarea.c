#include <pthread.h> // Incluye la biblioteca para trabajar con hilos
#include <stdio.h>   // Incluye la biblioteca estándar de entrada/salida
#include <stdlib.h>  // Incluye la biblioteca estándar de C

/* Programa que demuestra el paso de argumentos en la creación de threads */

// Defino el arreglo de mensajes
char *mensaje[2];

// Función que ejecutarán los hilos
void *imprimir(void *threadid) {
    int *id_ptr, taskid;
    id_ptr = (int *) threadid; // Convierte el puntero de argumento a un puntero a entero
    taskid = *id_ptr; // Obtiene el identificador de la tarea
    printf("Thread %d: %s\n", taskid, mensaje[taskid - 1]); // Imprime el mensaje correspondiente al hilo
    pthread_exit(NULL); // Finaliza el hilo
}

int main() {
    // Defino las variables para los hilos
    pthread_t hilo1, hilo2; // Identificadores de los hilos
    int *taskid1, *taskid2; // Punteros para los identificadores de tarea
    int nombre[2] = {1, 2}; // Arreglo con los identificadores de tarea
    pthread_attr_t attr; // Atributos de los hilos
    int rc; // Variable para códigos de retorno

    // Inicializo los mensajes
    mensaje[0] = "¡Soy el HILO 1!";
    mensaje[1] = "¡Soy el HILO 2!";

    // Imprimo mensaje del creador
    printf("\n Creador: creo 2 hilos y espero a que acaben\n\n");

    // Creo atributos de hilos
    pthread_attr_init(&attr); // Inicializa los atributos de los hilos
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE); // Establece el estado de separación a "joinable"

    // Asigno valores a los identificadores de tarea
    taskid1 = (int *) malloc(sizeof(int)); // Asigna memoria para el identificador de tarea 1
    *taskid1 = 1; // Asigna el valor 1 al identificador de tarea 1
    taskid2 = (int *) malloc(sizeof(int)); // Asigna memoria para el identificador de tarea 2
    *taskid2 = 2; // Asigna el valor 2 al identificador de tarea 2

    // Creo los hilos
    pthread_create(&hilo1, &attr, (void *)imprimir, (void *)taskid1); // Crea el hilo 1
    pthread_create(&hilo2, &attr, (void *)imprimir, (void *)taskid2); // Crea el hilo 2
    
    // Destruyo los atributos del hilo
    pthread_attr_destroy(&attr); // Destruye los atributos de los hilos

    // Espero que los hilos finalicen
    pthread_join(hilo1, NULL); // Espera a que el hilo 1 termine
    pthread_join(hilo2, NULL); // Espera a que el hilo 2 termine

    // Mensaje final del creador
    printf("Creador: los hilos terminaron y termino yo también\nAdiós...\n");

    // Finalizo el hilo principal
    pthread_exit(NULL); // Finaliza el hilo principal
}