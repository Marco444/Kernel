#ifndef COMMANDS_LISTS_H
#define COMMANDS_LISTS_H

#include "commands.h"
#include "commandsEngine.h"

/*
 * Definimos la manera de implementar nuestra tabla de comandos como un arreglo,
 * sin embargo consideramos oportuno mencionar que existen mejores estructuras
 * de datos. Un ejemplo clarisimo si tuvisiemos soporte para memoria dinamica
 * seria el uso de un Trie y asi al hacer la busqueda de un comando no
 * tendriamos que hacer el KMP de la string ingresada por el usuario con cada
 * entrada de nuestro arreglo de comandos, sino que hariamos directamente la
 * busqueda en el  (para un bosquejo ver en Trie.h)
 */
static Command commands[COMMANDS_COUNT] = {
    {"help", HELP_DESCRIPTION, help},
    {"diaYHora", DIA_Y_HORA_DESCRIPTION, diaYHora},
    {"divideByZero", DIVIDE_BY_ZERO_DESCRIPTION, divideByZero},
    {"invalidOpCode", INVALID_OPCODE_DESCRIPTION, invalidOpCode},
    {"printMem", PRINT_MEM_DESCRIPTION, printMem},
    {"infoReg", INFO_REG_DESCRIPTION, infoReg},
    {"primos", PRIMOS_DESCRIPTION, primos},
    {"fibonacci", FIBONACCI_DESCRIPTION, fibonacci},
    {"man", MAN_DESCRIPTION, man},
    {"clear", CLEAR_DESCRIPTION, clear},
    {"benchmark", BENCHMARK_DESCRIPTION, benchmark},
    {"pipes", PIPE_DUMP_DESCRIPTION, pipes}};
#endif
