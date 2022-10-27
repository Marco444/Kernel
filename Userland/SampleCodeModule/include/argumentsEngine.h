
#ifndef COMMANDSENGINE_ARGUMENTSENGINE_H
#define COMMANDSENGINE_ARGUMENTSENGINE_H

#include "lib.h"

/*
 * Este engine busca definir el manejo de los argumentos de los comandos,
 * el pasaje de la string luego del comando (y antes de comandos propios
 * del sistema operativo). Esencialmente toma la string de argumentos y
 * la transforma al formato que tienen los argumentos en Bash, el numero
 * de argumentos y un array de los argumentos como strings.
 *
 */

#define MAX_ARGUMENT 20
#define MAX_ARGUMENT_COUNT 20

#define ARGUMENT_LENGTH_EXCEEDED_MSG                                           \
  "Se ingreso un argumento demasiado largo, intente nuevamente \n"
#define ARGUMENT_COUNT_EXCEEDED_MSG                                            \
  "Se ingreso demasiados argumentos, intente nuevamente \n"

/*
 * Recibe una string con los argumentos recibos al haber llamado el comando
 * asi como una array de strings a la cual copiar los argumentos. Devuelve
 * el numero de argumentos que se leyeron a args.
 * */

int argumentsEngineHandle(Window window, char *arguments,
                          char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]);

#endif
