#include "include/mmuEngine.h"


void readMemoryTo(uint64_t * mem_address, int fd) {


	if((uint64_t) mem_address > (0x20000000 - 32)) ncPrintFD0("Invalid adress \n");
    
    uint8_t * aux = (uint8_t *) mem_address;

    for(int i=0; i < 32 ; ++i){
        ncPrintHexAtFD((uint64_t) aux, fd);
        ncPrintAtFD(" = ", fd);
        ncPrintHexAtFD(*aux, fd);
        ncPrintAtFD("   ", fd);
		if((i + 1) % 4 == 0) ncNewline(fd);
        ++aux;
    }
    
    ncNewline(fd);
}


void regsSnapshot(uint64_t * regs){

	for(int i = 0; i < REGS; i++){
		regsSaved[i] = regs[i];
	}

}

void getRegsSnapshot(uint64_t * buffer){

	for(int i = 0; i < REGS; i++){
		buffer[i] = regsSaved[i];
	}

}