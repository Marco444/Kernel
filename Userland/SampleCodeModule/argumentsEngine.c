// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/argumentsEngine.h"

#include "include/constants.h"
#include "include/lib.h"

/*
 * Este engine busca definir el manejo de los argumentos de los comandos,
 * el pasaje de la string luego del comando (y antes de comandos propios
 * del sistema operativo). Esencialmente toma la string de argumentos y
 * la transforma al formato que tienen los argumentos en Bash, el numero
 * de argumentos y un array de los argumentos como strings.
 *
 */
int argumentsEngineHandle(char *arguments,
                          char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {
  // we'll keep two pointers to copy each argument
  int currentArgument = 0, currentCharacter = 0;

  for (int i = 0; arguments[i] != NULL_ && arguments[i] != '\n'; i++) {
    while (arguments[i] != NULL_ && arguments[i] != SPACE) {
      argv[currentArgument][currentCharacter++] = arguments[i++];
      if (currentCharacter >= MAX_ARGUMENT) return -1;
    }

    argv[currentArgument][currentCharacter] = NULL_;
    currentCharacter = 0;
    currentArgument++;
    if (currentArgument >= MAX_ARGUMENT_COUNT) return -1;
  }
  return currentArgument;
}
