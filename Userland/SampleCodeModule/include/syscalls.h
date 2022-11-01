#ifndef SYSCALL_H
#define SYSCALL_H
#include <stdint.h>

#include "Windows.h"

extern void sysPsDump();

extern void sysPipesDump();

extern void *sysAlloc(int size);

extern void *sysMemoryDump(int size);

extern void *sysFree(const void *address);

extern void printMemFrom(unsigned int pos, Window window);

extern void sysWrite(int fd, char *buffer);

extern void sysRead(int fd, char *buffer);

extern void sysTime(char *buffer);

extern void waitPid(int pid);

extern void sysOpen(int fd);

extern void sysClose(int fd);

extern void exit(int fd);

extern int sysNiceProcess(int fd, int priority);

extern int sysKillProcess(int fd);

extern int sysBlockProcess(int fd);

extern int sysUnblockProcess(int fd);

extern void sysPauseProcess(int fd);

extern void sysReadMem(char *buffer, uint8_t *from, int cant);

extern void sysReloadProcess(int PID);

extern void sysClearScreen();

extern void sysWriteFormat(int fd, char *buffer, char format);

extern void sysGetRegsSnapshot(uint64_t *buffer);

extern void generateInvalidOpCode();

extern void divideByZeroAsm();

extern int SysProcesses();

typedef void *Semaphore;

extern Semaphore semOpen(int id, int value);

extern int semClose(Semaphore semaphore);

extern int semWait(Semaphore semaphore);

extern int semSignal(Semaphore semaphore);

extern void myYield();

#endif
