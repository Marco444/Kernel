// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.

#include "include/_stdlib.h"
#include "include/syscalls.h"

void free(const void *address) { sysFree(address); }

void *alloc(int size) { return sysAlloc(size); }

void memorydump(int window) { sysMemoryDump(window); }

void pipesdump() { sysPipesDump(); }

void psdump() { sysPsDump(); }

int dup2(int oldfd, int newfd) { return sysDup2(oldfd, newfd); }

int pipe(int fd[2]) { return sysPipe(fd); }

void close(int fd) { sysClose(fd); }
