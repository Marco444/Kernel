
#include "syscalls.h"

#define FOREGROUND 0
#define BACKGROUND 1

void free(const void *address);

void *alloc(int size);

void memorydump(int window);

void pipesdump();

void psdump();

int dup2(int oldfd, int newfd);

int pipe(int fd[2]);

void close(int fd);
