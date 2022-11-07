#ifndef COMMANDS_LISTS_H
#define COMMANDS_LISTS_H

#include "commands.h"
#include "commandsEngine.h"

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
    {"sem", SEM_DESCRIPTION, semStatus},
    {"loop", LOOP_DESCRIPTION, loop}};
#endif
