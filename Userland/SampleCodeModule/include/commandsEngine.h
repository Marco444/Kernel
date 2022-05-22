#ifndef COMMANDS_ENGINE_H
#define COMMANDS_ENGINE_H

/*

    La idea de este engine es poder manejar el mapeo de una input del usuario a la lista de comandos
    definidas. La verdad que para hacerlo bien se necesitaria una maquina de estados, sin embargo lo que proponemos
    aqui nos parece acertado.

    Dividimos los comandos segun su notacion, ya sea "OPERADOR ARG1, ARG2, .. ARGN" o "ARG1 OPERADOR ARG2.":

    Para manejar la primera propusimos algo analogo a como funciona correr un programa de C desde cualquier
    terminal, se reciben todos los argumentos luego del llamado al comando (asi como su numero).

    Para manejar la segunda propusimos directamente una unica funcion que chequea si esta el caracter '|' y
    llamar a una funcion especifica que maneja la operacion. En el caso que se desean incluir mas comandos
    con esta notacion (como por ejemplo el "<" de linux para escribir a archivo) simplemente habria que
    renombrar en los signature donde dice "Pipe" por "Special" y generalizar la funcion de runPipeCommand.

    Por ultimo, utilizamos una array de Commands para hacer el matching, sin embargo consideramos oportuno
    mencionar como la estructura de datos seria un Trie, que logramos implementar pero con la ayuda de malloc,
    cosa que no soporta de manera robusta nuestro sistema operativo actualmente.

*/

#include "Windows.h"
#include "commands.h"

#define INVALID_MSG "Invalid command, please try again \n"
#define ALREADY_SPLIT_MSG "Already in split view, cannot split again \n"

#define MAX_NAME 100
#define MAX_DESCRIPTION 300


//defino CommandPtr como un puntero a funcion de mis comandos
//(su signature siempre debe ser el mismo void que toman una window y
//una string con sus argumentos)
typedef void (*CommandPtr)(Window, int, char[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]);

typedef struct {
    char name[MAX_NAME];
    char description[MAX_DESCRIPTION];
    CommandPtr apply;
} Command;


static Command commands[] = {{"help",          HELP_DESCRIPTION,           help},
                             {"diaYHora",      DIA_Y_HORA_DESCRIPTION,     diaYHora},
                             {"divideByZero",  DIVIDE_BY_ZERO_DESCRIPTION, divideByZero},
                             {"invalidOpCode", INVALID_OPCODE_DESCRIPTION, invalidOpCode},
                             {"printMem",      PRINT_MEM_DESCRIPTION,      printMem},
                             {"infoReg",       INFO_REG_DESCRIPTION,       infoReg},
                             {"primos",        PRIMOS_DESCRIPTION,         primos},
                             {"fibonacci",     FIBONACCI_DESCRIPTION,      fibonacci},
                             {"man",           MAN_DESCRIPTION,            man}};

//OJO AL AGREGAR A LA LISTA DE ACTUALIZAR LA CANTIDAD DE COMANDOS

static int commandsCount = 9;


/*
 * Recibe un string como un supuesto comando y la mapea a la funcion correspondiente, 
 * que se correra en la ventana recibida como parametro.
 */
void commandsEngineHandle(char *command, Window window);

/*
 * Recibe un comando formato valido y una ventana a imprimirlo
 */
void commandsEngineRun(char *command, Window window);


/*
 * Recibe un comando pipe de formato valido y una ventana a imprimirlo
 */
void commandsEngineRunPipe(const char *command, Window window);


/*
 * Recibe un comando de formato valido y devuelve si es o no el comando pipe
 */
int isPipeCommand(const char *command);

/*
 * Recibe una window a la cual imprimir todos los comandos que actualmente maneja
 */
void commandsEngineDisplayCommands(Window window);

#endif