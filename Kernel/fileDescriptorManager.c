#include "include/fileDescriptorManager.h"
#include "include/keyBoard.h"
#include "include/list.h"
#include "include/naiveConsole.h"
#include "include/pipeEngine.h"
#include "include/schedluerEngine.h"
#include <string.h>

/* mantengo una lista con todos los fd del sistema */
struct fdEngine {
  File fds[MAX_FD_COUNT];
  int next;
};

struct fdEngine FdEngine;

struct file _stdin, _stdout;

File *allocFileDescriptor() {
  if (FdEngine.next > MAX_FD_COUNT)
    return NULL;
  return &FdEngine.fds[FdEngine.next++];
}

void initFileDescriptorEngine() { FdEngine.next = 0; }

File getstdin() { return &_stdin; }

File getstdout() { return &_stdout; }

void sysWrite(char *buffer) {
  ncPrint(buffer);
  // pipewrite(currentProcess->fd[1]->pipe, buffer, strlen(buffer));
}

void sysRead(char *buffer) { getBufferChar(buffer); }

void close(struct file *fd) { pipeclose(fd->pipe, fd->writable); }
