#include "include/fileDescriptorManager.h"
#include "include/keyBoard.h"
#include "include/list.h"
#include "include/naiveConsole.h"
#include "include/pipeEngine.h"
#include "include/schedluerEngine.h"

struct fdEngine FdEngine;

void dup2(int pid, int oldfd, int newfd) {
  ncPrint("dup2 called");
  // deberia ir en el scheduler
}

File allocFileDescriptor() {
  if (FdEngine.next >= MAX_FD_COUNT)
    return NULL;
  return &FdEngine.fds[FdEngine.next++];
}

void initFdManager() {
  FdEngine.next = 2;
  for (int i = 2; i < MAX_FD_COUNT; i++)
    FdEngine.fds[i].id = i;
  initPipeEngine();
}

void sysWrite(char *buffer) {
  ncPrint(buffer);
  // pipewrite(currentProcess->fd[1]->pipe, buffer, strlen(buffer));
}

void sysRead(char *buffer) { getBufferChar(buffer); }

void close(struct file *fd) { pipeclose(fd->pipe, fd->writable); }
