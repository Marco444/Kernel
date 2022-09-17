#ifndef EXCEPTION_ENGINE_H
#define EXCEPTION_ENGINE_H

#include "naiveConsole.h"
/*
    *Constante la cual define el codigo de error de dividir por 0
*/
#define ZERO_EXCEPTION_ID 0
/*
    *Constante la cual nos indica el error de invalid Opcode
*/
#define INVALID_OPCODE_ID 6
/*
    *Constante la cual nos dice la cantidad de registros que tenemos
*/
#define REGS_CANT 16
/*
    * Funcion la cual se encarga de manejar que se imprime cuando llego un error de zero_division
*/

static void zero_division(long regs[],char fd);
/*
    *Funcion la cual se encarga de imprimir cuando se lanzo la excepcion de invalid_opcode
*/
static void invalid_opcode(long regs[],char fd);

/*
    *Funcion que se encuentra en el schedulerEngine la cual nos devuelve el FD
*/
extern int getFD(int contextOwner);


#endif
