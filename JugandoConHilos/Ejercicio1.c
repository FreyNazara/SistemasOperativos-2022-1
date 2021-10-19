#include "Ejercicio1.h"

void *imprimirMensaje(void *ptr){
    char *mensaje;
    mensaje = (char *) ptr;
    printf("%s \n", mensaje);
    sleep(10);
}

int main()
{
    pthread_t thread1, thread2, thread3;
    const char *mensaje1 = "1 + 2";
    const char *mensaje2 = "Soy el hilo 2 ";
    const char *mensaje3 = "Me perdí en la ejecución ";
    int ret1, ret2, ret3;
    printf("Hilo principal crando al hilo 1\n");
    ret1 = pthread_create( &thread1, NULL, imprimirMensaje, (void*) mensaje1);
    if(ret1 != 0){
        printf("Error - pthread_create() codigo de error: %d\n", ret1);
        exit(EXIT_FAILURE);
    }
    printf("Hilo principal creando al hilo 2\n");
    ret2 = pthread_create( &thread2, NULL, imprimirMensaje, (void*) mensaje2);
    if(ret2 != 0){
        printf("Error - pthread_create() codigo de error: %d\n", ret2);
        exit(EXIT_FAILURE);
    }
    printf("Hilo principal creando al hilo 3\n");
    ret3 = pthread_create( &thread3, NULL, imprimirMensaje, (void*) mensaje3);
    if(ret3 != 0){
        printf("Error - pthread_create() codigo de error: %d\n", ret3);
        exit(EXIT_FAILURE);
    }
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
    printf("Hilo principal saliendo\n");
    exit(EXIT_SUCCESS);
}
