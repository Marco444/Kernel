#ifndef SYSCALL_H
#define SYSCALL_H
#include <stdint.h>

#include "Windows.h"

extern void sysPipesDump();

extern void *sysAlloc(int size);

extern void *sysMemoryDump(int size);

extern void *sysFree(const void *address);

extern void printMemFrom(unsigned int pos, Window window);

extern void sysWrite(int fd, char *buffer);

extern void sysRead(int fd, char *buffer);

extern void sysTime(char *buffer);

extern void sysOpen(int fd);

extern void sysClose(int fd);

extern void exit(int fd);

extern void sysKillProcess(int fd);
extern void sysPauseProcess(int fd);
extern void sysReadMem(char *buffer, uint8_t *from, int cant);

extern void sysReloadProcess(int PID);

extern void sysClearScreen(int fd);

extern void sysWriteFormat(int fd, char *buffer, char format);

extern void sysGetRegsSnapshot(uint64_t *buffer);

extern void generateInvalidOpCode();

extern void divideByZeroAsm();

extern int SysProcesses();
#endif
