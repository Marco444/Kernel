#include "include/commandsEngine.h"
#include "include/lib.h"

void commandsEngineDisplayCommands(Window window) {
    for (int i = 0; i < commandsCount; ++i) {
        puts_("     ", window);
        puts_(commands[i].name, window);
        puts_(" : ", window);
        puts_(commands[i].description, window);
        newLine(window);
    }
}

void commandsEngineHandle(char *command, Window window) {

    //chequeo default de NULL_ y el tamaño del comando a leer
    if (command == NULL_ || isLongerThan(command, MAX_COMMAND_SIZE)) return;

    //chequeo si tiene un pipe, analogo al manejo de "<" para comandos especiales.
    if (isPipeCommand(command)) commandsEngineRunPipe(command, window);

        //le digo al engine de comandos que lo corra en la window
    else commandsEngineRun(command, window);
}


void commandsEngineRunPipe(const char *command, Window window) {

    //chequeo si puedo hacer el comando pipe
    if (window != MAIN_WINDOW) {
        puts_(ALREADY_SPLIT_MSG, window);
        return;
    }

    //defino dos buffers para copiar los dos argumentos del comando
    //pipe, observar que ya chequee que entra en su enteridad la
    //string command en un array de MAX_COMMAND_SIZE
    char cmd1[MAX_COMMAND_SIZE], cmd2[MAX_COMMAND_SIZE];

    int i = 0, dim1 = 0, dim2 = 0;

    //copio todos los caracteres antes del pipe
    while (command[i] != NULL_ && command[i] != PIPE) cmd1[dim1++] = command[i++];

    //no copio el pipe a ningun comando
    i++;

    //copio hasta el final de la string al segundo comando
    while (command[i] != NULL_) cmd2[dim2++] = command[i++];


    commandsEngineRun(cmd1, LEFT_WINDOW);
    commandsEngineRun(cmd2, RIGHT_WINDOW);
}

void commandsEngineRun(char *command, Window window) {

    //remuevo los espacios y tabs que rodean al comando
    command += removeTrailingSpaces(command);

    int found = 0;

    //voy por todos los comandos y chequeo que comando lo tengo
    //como una substring de lo pasado como argumento que pase,
    //ejemplo command: "printMem 500", chequeo si "printMem"
    //es una substring comenzando en indice cero
    for (int i = 0; i < commandsCount; i++) {
        if (substring(command, commands[i].name) == 0) {

            found = 1;

            //guardo el offset para tener donde comienzan los
            //argumentos de mi comando
            int argumentsBeginAtOffset = strlen_(commands[i].name);
            command += argumentsBeginAtOffset;

            //Aca lo mejor seria hacerlo con memoria dinamica pero
            //nuestro kernel no maneja mallocs y frees
            char args[MAX_ARGUMENT_COUNT][MAX_ARGUMENT];
            int argc = argumentsEngineHandle(window, command, args);

            //hago la syscall de manejo de context switching, ojo
            //que tengo que manejar los argumentos, que tambien deben
            //ir copiados sobre la posicion de memoria de la funcion.
            CommandPtr cmd = commands[i].apply;
            cmd(window, argc, args); //(por ahora simplemente la llamo)
        }
    }

    if (!found) puts_(INVALID_MSG, window);
}


int isPipeCommand(const char *command) {

    for (int i = 0; command[i] != NULL_; i++)
        if (command[i] == PIPE)
            return 1;

    return 0;
}