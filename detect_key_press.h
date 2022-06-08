#ifndef DETECT_KEY_PRESS_H
#define DETECT_KEY_PRESS_H

#include<stdio.h>
#include <termios.h>    //termios, TCSANOW, ECHO, ICANON
#include <unistd.h>     //STDIN_FILENO

char kbhit(){   
    // printf("\t\t[+]Entrando a KBHIT: ");
    int c;   
    static struct termios oldt, newt;

    /* tcgetattr obtiene la configuración actual del terminal
       STDIN_FILENO se utiliza para escribir la configuración en oldt */
    tcgetattr(STDIN_FILENO, &oldt);
    /* se hace una copia de la configuración */
    newt = oldt;

    /* se deshabilita el flag ICANON */
    newt.c_lflag &= ~(ICANON);          

    /* se envia la nueva configuración a STDIN
       usamos TCSANOW para modificar la configuración. */
    tcsetattr( STDIN_FILENO, TCSANOW, &newt);

    /* En este caso se usa la tecla '1 - 4' sale del ciclo for y retorna la tecla */
    while((c=getchar())){
        // putchar(c);                 
        // printf("\n\n%c\n\n", c);
        /* se restaura la configuración original */
        tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
        return c;
    }    
}

#endif