#include <pthread.h> // Incluye la biblioteca para trabajar con hilos
#include <stdio.h>   // Incluye la biblioteca estándar de entrada/salida
#include <stdlib.h>  // Incluye la biblioteca estándar de C

/* Programa que demuestra el paso de argumentos en la creación de threads con orden controlado */

// Defino el arreglo de mensajes
char *mensaje[5];

// Variables globales para controlar la sincronización
int turno = 1;  // Variable que indica el turno de impresión de cada hilo
pthread_mutex_t mutex;  // Mutex para proteger el acceso a la variable "turno"
pthread_cond_t cond;    // Condicional para despertar a los hilos en orden

// Función que ejecutarán los hilos
void *imprimir(void *threadid) {
    int *id_ptr, taskid;
    id_ptr = (int *) threadid; // Transforma el puntero de argumento a un puntero a entero
    taskid = *id_ptr; // Otorga el identificador de la tarea

    // Control de sincronización para imprimir en el orden deseado
    pthread_mutex_lock(&mutex);  // Bloquea el mutex

    while (taskid != turno) {
        pthread_cond_wait(&cond, &mutex);  // Espera hasta que sea su turno
    }

    printf("Thread %d: %s\n", taskid, mensaje[taskid - 1]); // Imprime el mensaje correspondiente a su respectivo hilo

    // Actualiza el turno según el orden especificado: 1, 3, 2, 5, 4
    if (turno == 1) turno = 3;
    else if (turno == 3) turno = 2;
    else if (turno == 2) turno = 5;
    else if (turno == 5) turno = 4;
    else turno = 0;  // Finalización

    pthread_cond_broadcast(&cond);  // Despierta a los otros hilos para que revisen si es su turno
    pthread_mutex_unlock(&mutex);  // Desbloquea el mutex

    pthread_exit(NULL); // Finaliza el hilo
}

int main() {
    // Definición las variables para los hilos
    pthread_t hilos[5]; // Identificadores de cada uno de los hilos
    int *taskid[5]; // Punteros para los identificadores de las tareas
    pthread_attr_t attr; // Atributos para los hilos
    int rc; // Variable para códigos de retorno

    // Inicializo los mensajes con los siguientes arreglos
    mensaje[0] = "¡Soy el HILO 1!";
    mensaje[1] = "¡Soy el HILO 2!";
    mensaje[2] = "¡Soy el HILO 3!";
    mensaje[3] = "¡Soy el HILO 4!";
    mensaje[4] = "¡Soy el HILO 5!";

    // Inicialización del mutex y condicional
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    // Imprimir el mensaje del creador de los hilos
    printf("\nCreador: creo 5 hilos y espero a que acaben\n\n");

    // Creo los atributos de los hilos
    pthread_attr_init(&attr); // Inicializa los atributos de los hilos
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE); // Establece el estado de separación a "joinable"

    // Asigna valores a los identificadores de las tareas y crea los hilos
    for (int i = 0; i < 5; i++) {
        taskid[i] = (int *) malloc(sizeof(int)); // Asigna memoria para el identificador de tarea
        *taskid[i] = i + 1; // Asigna el valor al identificador de la tarea
        pthread_create(&hilos[i], &attr, (void *)imprimir, (void *)taskid[i]); // Crea el hilo
    }

    // Elimino los atributos del hilo
    pthread_attr_destroy(&attr);

    // Espero que los hilos finalicen
    for (int i = 0; i < 5; i++) {
        pthread_join(hilos[i], NULL); // Que cada hilo termine
    }

    // Mensaje final 
    printf("Creador: los hilos terminaron y termino yo también\nAdiós...\n");

    // Destruye el mutex y la variable condicional
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    // Finalizo 
    pthread_exit(NULL);
}

