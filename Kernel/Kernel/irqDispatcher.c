// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <keyBoard.h>
#include <lib.h>
#include <naiveConsole.h>
#include <stdint.h>
#include <timeDriver.h>

#include "../include/fileDescriptorManager.h"
#include "../include/regsSnahpshot.h"
#include "../include/semaphores.h"

extern void readMemory(char *buffer, int from, int qty);

void irqDispatcher(uint64_t irq) {}

void int_21(uint64_t *regs) {  // Llamamos al driver del teclado para que guarde
                               // en su buffer
  int c = readKey();

  if (getValue(c) == '=') {
    regsSnapshot(regs);
    return;
  }

  saveBuffer(c);
}

void syscalls(int fd, char *sysBuffer, int count, int num) {
  switch (num) {
    case 120:
      time_syscall(sysBuffer);
      break;

    case 121:
      fdClear(fd);
      break;

    case 123:
      readMemory(sysBuffer, fd, count);
      break;

    case 124:
      getRegsSnapshot((uint64_t *)sysBuffer);
      break;

    case 125:
      ncPrintHeader(sysBuffer, count);
      break;

    case 1:
      sysWriteFormat(fd, sysBuffer, count);
      break;
    case 0:
      sysRead(fd, sysBuffer);
      break;
    default:
      break;
  }
}
