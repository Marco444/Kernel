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
    {"block", BLOCK_DESCRIPTION, block},
    {"cat", CAT_DESCRIPTION, cat},
    {"clear", CLEAR_DESCRIPTION, clear},
    {"diaYHora", DIA_Y_HORA_DESCRIPTION, diaYHora},
    {"divideByZero", DIVIDE_BY_ZERO_DESCRIPTION, divideByZero},
    {"fibonacci", FIBONACCI_DESCRIPTION, fibonacci},
    {"filter", FILTER_DESCRIPTION, filter},
    {"help", HELP_DESCRIPTION, help},
    {"infoReg", INFO_REG_DESCRIPTION, infoReg},
    {"invalidOpCode", INVALID_OPCODE_DESCRIPTION, invalidOpCode},
    {"kill", KILL_DESCRIPTION, kill},
    {"man", MAN_DESCRIPTION, man},
    {"mem", MEM_DESCRIPTION, mem},
    {"nice", NICE_DESCRIPTION, nice},
    {"pipes", PIPE_DUMP_DESCRIPTION, pipes},
    {"primos", PRIMOS_DESCRIPTION, primos},
    {"printMem", PRINT_MEM_DESCRIPTION, printMem},
    {"ps", PS_DUMP_DESCRIPTION, ps},
    {"test", TEST_MANAGER_DESCRIPTION, testManager},
    {"wc", WC_DESCRIPTION, wc},
    {"phylo", PHYLO_DESCRIPTION, initializePhylos},
    {"sem", SEM_DESCRIPTION, semStatus}};
#endif
