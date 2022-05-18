#ifndef WINDOWS_H
#define WINDOWS_H

#include "../lib/lib.h"

#define MAX_COMMAND_SIZE 120

/*
 * Enumerador con los tipos de ventanas posibles.
 */
enum window_ {MAIN_WINDOW = 0, LEFT_WINDOW, RIGHT_WINDOW};
typedef enum window_ Window;


/*
 * Recibe una ventana la cual va a escuchar al teclado e imprimir en pantalla.
 */
void listen(char* command, Window window);


/*
 * Recibe una ventana en la cual inicializara una terminal de comandos
 */
void windowStart(Window window);


static void printShellLegend(Window window){
    puts("user@device: ~$ ", window);
}


#endif