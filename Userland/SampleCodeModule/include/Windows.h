#ifndef WINDOWS_H
#define WINDOWS_H

#define MAX_COMMAND_SIZE 120
#define SHELL_LEGEND "user@device: ~$ "

#include "constants.h"

/*
 * Recibe una ventana la cual va a escuchar al teclado e imprimir en pantalla.
 */
void listen(char *command, Window window);

/*
 * Recibe una ventana en la cual inicializara una terminal de comandos
 */
void windowStart(Window window);

#endif
