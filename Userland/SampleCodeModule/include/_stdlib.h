
#include "syscalls.h"

void free(const void *address);

void *alloc(int size);

void memorydump(int window);

void pipesdump();

void psdump();

void dup2(int pid, int oldfd, int newfd);

void pipe(int fd[2]);
