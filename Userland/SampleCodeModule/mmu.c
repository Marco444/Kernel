// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/mmu.h"

void sysTime(char *buffer);

void sysGetRegsSnapshot(uint64_t *buffer);

void getTime(char *buffer) { sysTime(buffer); }

extern void getRegs(long *buffer);

void getRegsSnapshot(uint64_t *buffer) { sysGetRegsSnapshot(buffer); }
