
#include "syscalls.h"

void free(const void *address);

void *alloc(int size);

void memorydump(int window);

void pipesdump();

void psdump();

int dup2(int oldfd, int newfd);

int pipe(int fd[2]);
