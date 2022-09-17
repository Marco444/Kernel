#include "include/mmu.h"


void sysTime(char * buffer);

void sysGetRegsSnapshot(uint64_t * buffer);

void getTime(char * buffer){
    sysTime(buffer);
}

extern void getRegs(long * buffer);

void getRegsSnapshot(uint64_t * buffer){
    sysGetRegsSnapshot(buffer);
}
