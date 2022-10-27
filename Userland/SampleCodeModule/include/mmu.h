#include <stdint.h>

#define REGS_CANT 17

/*
 * Retorna un string con la hora
 */
void getTime(char *buffer);

/*
 * Retorna una copia de los valores de los registros,
 * como un arreglo de caracteres. En el orden definido por
 *  {"RIP", "RAX", "RBX", "RCX", "RDX", "RSI", "RDI", "RBP", "RSP",
 *  "R8 ", "R9 ", "R10", "R11", "R12", "R13", "R14", "R15"};
 */
void readRegs(long *buffer);

/*
 * Retorna un snapshot de los registros en un momento previo
 * como un arreglo de unit64.
 *  ??ORDEN??{"RIP", "RAX", "RBX", "RCX", "RDX", "RSI", "RDI", "RBP", "RSP",
 *  "R8 ", "R9 ", "R10", "R11", "R12", "R13", "R14", "R15"};
 */
void getRegsSnapshot(uint64_t *buffer);