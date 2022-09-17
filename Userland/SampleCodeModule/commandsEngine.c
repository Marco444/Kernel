#include "include/commandsEngine.h"
#include "include/lib.h"
#include "include/WindowsEngine.h"
#include "include/commandsLists.h"



///////////////////////////////////////////////////
// Aux declarations                             //
/////////////////////////////////////////////////
int loadProcess(CommandPtr cmd, Window window, int argc, char *args[]);


///////////////////////////////////////////////////
// Used to be in header                         //
/////////////////////////////////////////////////




void commandsEngineHandle(char *command, Window window) {

    //chequeo default de NULL_ y el tamaño del comando a leer
    //obs. utilizo isLongerThan y no un strlen por el hecho que no se el 
    //size del input y podria presentar una vulnerabilidad de overflow
    if (command == NULL_ || isLongerThan(command, MAX_COMMAND_SIZE)) return;

    //como el pipe es un comando especial, se maneja diferente
    if (isPipeCommand(command)) commandsEngineRunPipe(command, window);

    //le digo al engine de comandos que lo corra en la window
    else {
        commandsEngineRun(command, window);
        waitProcessMain();
    }
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

    //limpio la pantalla porque voy a dividirla
    sysClearScreen(window);

    //el motor se encarga de levantar el cmd1 en la ventana izquierda
    commandsEngineRun(cmd1, LEFT_WINDOW);

    //el motor se encarga de levantar el cmd2 en la ventana derecha
    commandsEngineRun(cmd2, RIGHT_WINDOW);
    
    //espero a las interrupciones de teclado del usuario 

    waitProcessPipe();
}

void commandsEngineRun(char *command, Window window) {

    //remuevo los espacios y tabs que rodean al comando
    command += removeTrailingSpaces(command);

    int found = 0;

    //voy por todos los comandos y chequeo que comando lo tengo
    //como una substring de lo pasado como argumento que pase,
    //ejemplo command: "printMem 500", chequeo si "printMem"
    //es una substring comenzando en indice cero (aca entraria
    //un trie)
    for (int i = 0; i < COMMANDS_COUNT; i++) {
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

            //Por ultimo, cargo el puntero a funcion a la tabla de
            //context switching del kernel a traves de la syscall
            //que ejecuta loadProcess
            CommandPtr cmd = commands[i].apply;
            loadProcess(cmd, window, argc, (char **)args);
        }
    }

    if (!found) puts_(INVALID_MSG, window);
}

void printCommand(Window window, char * name, char * description) {
    puts_(DOUBLE_TAB, window);
    putsf_(name, LIGHT_CYAN, window);
    puts_(" : ", window);
    puts_(description, window);
    newLine(window);
}

void commandsEngineDisplayCommands(Window window) {

    //imprimo todos los comandos normales
    for (int i = 0; i < COMMANDS_COUNT; ++i) {
        printCommand(window, commands[i].name, commands[i].description);
    }

    //imprimo todos los comandos especiales
    //como solo es uno lo hago asi, sino crearia otro arreglo con ellos
    printCommand(window, PIPE_CMD, PIPE_DESCRIPTION);
}

int isPipeCommand(const char *command) {

    for (int i = 0; command[i] != NULL_; i++)
        if (command[i] == PIPE)
            return 1;

    return 0;
}
