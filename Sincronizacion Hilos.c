/*
Práctica 1. Sincronización de hilos, canales 
Implementar un programa en C/C++ con estándar POSIX para ejecución concurrente (usando hilos) que almacene las muestras de cuatro señales generadas por tareas diferentes. Los datos generados por las tareas deben almacenarse en un archivo txt donde columnas representan las muestras de cada señal. Simultáneamente, se debe monitorear el teclado para pausar, reanudar y detener el proceso. Finalmente, se debe incluir el desplegado de mensajes del estado del proceso que pueden ser: iniciado, pausado y reanudado. Las señales a generar son seno, coseno, triangular y cuadrada cuyo número de muestras por ciclo es de 100.
Criterio de Evaluación:
Cumplimiento de todos los requisitos: 60%
Mejora de confiabilidad del programa( prevenir errores del usuario): 30%
Funciones adicionales(por lo menos una): 10%
Forma de entrega: Preparar un video explicativo que muestre el código fuente y la demostración del funcionamiento del programa. La duración del video no debe exceder los tres minutos. Al terminar, guarda el video en una plataforma para compartir documentos o videos como Drive, OneDrive, Dropbox incluso Youtube. Finalmente, escribe el enlace al video antes de la fecha límite.
Nota: Si el código fuente presenta un porcentaje de similitud con el de otro compañero mayor del 70%, se considerará como trabajo plagiado y se calificará con 0 puntos a todos los que resulten similares. Para medir el porcentaje de similitud se utilizará un programa informático para Moodle.
*/
#include "ecuaciones.h"
#include "detect_key_press.h"

#define TAM_BFF 100
#define TAM 6
sem_t mutex_sem;
static int status = 0;
static char tecla;
char write_data[TAM_BFF];

void options(){
    printf("\tBienvenido al programa de STR...\nPresione 1 para empezar el programa\n");
    printf("1. Empezar el programa\n");
    printf("2. Interrumpir el programa\n");
    printf("3. Restablecer el programa\n");
    printf("4. Salir del programa\n\n>>");
}

void *controlador_teclado(void *arg){
    sem_wait(&mutex_sem);
    while(1){
        system("clear");
        options();
        while(tecla = kbhit()){
            system("clear");
            options();
            if(tecla == '1' && status != 1){
                status = 1; contador = 0;
                printf(">> Empezando el programa el programa. "); 
                sem_post(&mutex_sem);
            }else if(tecla == '2' && status != 0){
                status = 0;
                printf(">> Interrumpiendo el programa el programa. "); 
                sem_wait(&mutex_sem);
            }else if(tecla == '3' && status != 1){
                status = 1;
                printf(">> Reaunudando el programa el programa. "); 
                sem_post(&mutex_sem);
            }else if(tecla == '4'){
                printf(">> Saliendo del programa...\n");
                exit(4);
            }
        }
    }
}

void *write_file(void * arg){
    int fd;
    mkfifo("mi_fifo.txt", 666);
    fd = open("mi_fifo.txt", O_WRONLY|O_CREAT);
        for(int i = 0; i <= NUM_MUESTRAS; i++){
            pthread_mutex_lock(&mutex);

            gcvt(datos_funciones[0][i], TAM, write_data);
            write(fd, write_data, strlen(write_data));
            write(fd, "\t\t", strlen("\t\t"));

            gcvt(datos_funciones[1][i], TAM, write_data);
            write(fd, write_data, strlen(write_data));
            write(fd, "\t\t", strlen("\t\t"));

            gcvt(datos_funciones[2][i], TAM, write_data);
            write(fd, write_data, strlen(write_data));
            write(fd, "\t\t", strlen("\t\t"));

            gcvt(datos_funciones[3][i], TAM, write_data);
            write(fd, write_data, strlen(write_data));
            write(fd, "\t\t\n", sizeof("\t\t\n"));

            pthread_mutex_unlock(&mutex);
        }
    close(fd);
}

void *controlador_principal(void *arg){
    pthread_t threads[NUM_THRD];
    pthread_t pipe_threads[2];

    while(1){
        sem_wait(&mutex_sem);
        if(pthread_create(&threads[0], NULL, func_seno, NULL)){ printf("Error en la creacion del hilo[0]"); exit(1); }
        if(pthread_create(&threads[1], NULL, func_coseno, NULL)){ printf("Error en la creacion del hilo[1]"); exit(1); }
        if(pthread_create(&threads[2], NULL, func_triangular, NULL)){ printf("Error en la creacion del hilo[2]"); exit(1); }
        if(pthread_create(&threads[3], NULL, func_cuadrada, NULL)){ printf("Error en la creacion del hilo[3]"); exit(1); }
        if(pthread_create(&pipe_threads[0], NULL, write_file, NULL)){ printf("Error en la creacion del hilo pipe_threads"); exit(1); }
        for(int i = 0; i < NUM_THRD; i++){ pthread_join(threads[i], NULL); }
        pthread_join(pipe_threads[0], NULL);
        sem_post(&mutex_sem); printf("\r");
    }
}

int main(int argc, char *argv[]){
    // Creando numero de hilos
    pthread_t thread_principal, thread_teclado;
    // Declarando el semaforo
    sem_init(&mutex_sem, 0 ,1);
   
    // Creando la funcion principal y lectura del teclado
    pthread_create(&thread_teclado, NULL, controlador_teclado, NULL);
    pthread_create(&thread_principal, NULL, controlador_principal, NULL);

    pthread_join(thread_principal, NULL);
    pthread_join(thread_teclado, NULL);

    return 0;
}
