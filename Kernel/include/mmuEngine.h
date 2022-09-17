
#ifndef MMUENGINE_H_
#define MMUENGINE_H_
#include <lib.h>
#include <naiveConsole.h>
/*
 * Este ADT se encarga de obtener datos del hardware a traves del modo protegido de intel.
 */

#define REGS 18





void readMemoryTo(uint64_t * mem_address, int fd);

void regsSnapshot(uint64_t * regs);

void getRegsSnapshot(uint64_t * buffer);
#endif