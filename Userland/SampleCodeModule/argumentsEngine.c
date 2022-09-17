#include "include/argumentsEngine.h"
#include "include/lib.h"

/*  
 * Este engine busca definir el manejo de los argumentos de los comandos,
 * el pasaje de la string luego del comando (y antes de comandos propios
 * del sistema operativo). Esencialmente toma la string de argumentos y
 * la transforma al formato que tienen los argumentos en Bash, el numero
 * de argumentos y un array de los argumentos como strings.
 *
*/

int argumentsEngineHandle(Window window, char *arguments, char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]) {

    //we'll keep two pointers to copy each argument
    int currentArgument = 0, currentCharacter = 0, i = 0;

    while (arguments[i] != NULL_) {

        //copio al currentArgument todos los caracteres que no son
        //espacios ni tabs
        while (arguments[i] != NULL_ && arguments[i] != TAB && arguments[i] != SPACE) {
            argv[currentArgument][currentCharacter++] = arguments[i++];
            if (currentCharacter >= MAX_ARGUMENT_COUNT) {
                puts_(ARGUMENT_LENGTH_EXCEEDED_MSG, window);
                return -1;
            }
        }

        argv[currentArgument][currentCharacter] = NULL_;



        //si leo tabs o espacio los salteo
        while (arguments[i] != NULL_ && (arguments[i] == TAB || arguments[i] == SPACE))
            i++;
        

        //aca agrego uno al numero de argumentos porque o termine (y recordar que
        //empeze en cero) o
        currentArgument++;

        if (currentArgument >= MAX_ARGUMENT_COUNT) {
            puts_(ARGUMENT_COUNT_EXCEEDED_MSG, window);
            return -1;
        }
    }

    return currentArgument;
}

