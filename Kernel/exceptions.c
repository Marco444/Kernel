// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "include/exceptionsEngine.h"

static const char *registerNames[REGS_CANT + 1] = {
    "RAX", "RBX", "RCX", "RDX", "RSI", "RDI", "RBP", "RSP", "R8 ",
    "R9 ", "R10", "R11", "R12", "R13", "R14", "R15", "RIP"};

static void reportRegs(long regs[], char fd) {
  ncPrint("    -Exception at -> RIP: ");
  ncPrintHex(regs[REGS_CANT]);
  ncPrint("\n");
  for (int i = 0; i < REGS_CANT; i++) {
    ncPrint("    -");
    ncPrint(registerNames[i]);
    ncPrint(": ");
    ncPrintHex(regs[i]);
    ncPrint("\n");
  }
}

void exceptionDispatcher(int exception, long regs[], char *contextOwner) {
  int fd = getFD((int)*contextOwner);
  fdClear(fd);

  ncClearHeader();
  ncPrintHeader(
      "                                   EXCEPTION                  "
      "                  ",
      LIGHT_RED_BACKGROUND | WHITE);

  switch (exception) {
    case ZERO_EXCEPTION_ID:
      zero_division(regs, fd);
      break;

    case INVALID_OPCODE_ID:
      invalid_opcode(regs, fd);
      break;

    default:
      break;
  }
}

static void zero_division(long regs[], char fd) { reportRegs(regs, fd); }

static void invalid_opcode(long regs[], char fd) { reportRegs(regs, fd); }
