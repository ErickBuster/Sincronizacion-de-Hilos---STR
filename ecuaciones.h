#ifndef ECUACIONES_H
#define ECUACIONES_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h> 
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

#define NUM_MUESTRAS 100
#define NUM_THRD 4
#define PI 3.1415926535897932

static int contador = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
float datos_funciones[NUM_THRD][NUM_MUESTRAS];

void *func_seno(void *arg){
    for(int  i = 0; i <= NUM_MUESTRAS; i++){
        pthread_mutex_lock(&mutex);
        datos_funciones[0][i] = sin((i * PI) / (NUM_MUESTRAS/2));
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

void *func_coseno(void *arg){
    for(int  i = 0; i <= NUM_MUESTRAS; i++){
        pthread_mutex_lock(&mutex);
        datos_funciones[1][i] = cos((i * PI) / (NUM_MUESTRAS/2));
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}
void *func_triangular(void *arg){
    for(int  i = 0; i <= NUM_MUESTRAS; i++){
        pthread_mutex_lock(&mutex);
        if(i < (NUM_MUESTRAS/2)){
            datos_funciones[2][i] = i;
        }else{
            datos_funciones[2][i] = NUM_MUESTRAS - i;
        }
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}
void *func_cuadrada(void *arg){
    for(int  i = 0; i <= NUM_MUESTRAS; i++){
        pthread_mutex_lock(&mutex);
        if(i < (NUM_MUESTRAS/2)){
            datos_funciones[3][i] = 0;
        }else{
            datos_funciones[3][i] = 1;
        }
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

#endif