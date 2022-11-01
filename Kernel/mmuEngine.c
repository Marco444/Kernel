// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/mmuEngine.h"

uint64_t regsSaved[200] = {0};
void readMemoryTo(uint64_t *mem_address, int fd) {

  if ((uint64_t)mem_address > (0x20000000 - 32))
    ncPrintFD0("Invalid adress \n");

  uint8_t *aux = (uint8_t *)mem_address;

  for (int i = 0; i < 32; ++i) {
    ncPrintHexAtFD((uint64_t)aux);
    ncPrintAtFD(" = ");
    ncPrintHexAtFD(*aux);
    ncPrintAtFD("   ");
    if ((i + 1) % 4 == 0)
      ncNewline();
    ++aux;
  }

  ncNewline(fd);
}

void regsSnapshot(uint64_t *regs) {

  for (int i = 0; i < REGS; i++) {
    regsSaved[i] = regs[i];
  }
}

void getRegsSnapshot(uint64_t *buffer) {

  for (int i = 0; i < REGS; i++) {
    buffer[i] = regsSaved[i];
  }
}