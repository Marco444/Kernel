
#include "include/_stdlib.h"
#include "include/syscalls.h"

void free(const void *address) { sysFree(address); }

void *alloc(int size) { return sysAlloc(size); }

void memorydump(int window) { sysMemoryDump(window); }

void pipesdump() { sysPipesDump(); }

void psdump() { sysPsDump(); }

void dup2(int pid, int oldfd, int newfd) { return; }

void pipe(int fd[2]) { return; }
