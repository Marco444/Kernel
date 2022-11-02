#include "include/fileDescriptorManager.h"
#include "include/keyBoard.h"
#include "include/lib.h"
#include "include/list.h"
#include "include/naiveConsole.h"
#include "include/pipeEngine.h"
#include "include/schedluerEngine.h"

#define MSG_ERROR_READING "Error reading from fd \n"
#define MSG_ERROR_WRITING "Error writing to fd \n"

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
  for (int i = 0; i < MAX_FD_COUNT; i++)
    FdEngine.fds[i].id = i;
  initPipeEngine();
}

int findProcessFd(int fd) {
  if (fd < 0 || fd >= MAX_FD_PROCESS)
    return -1;

  int *fdIds = currentProcessFds();
  return fdIds[fd];
}

void sysWrite(int fd, char *buffer) {
  int fdId = findProcessFd(fd);

  if (fdId < 0)
    ncPrint(MSG_ERROR_WRITING);
  else if (FdEngine.fds[fdId].id == STDOUT)
    ncPrint(buffer);
  else
    pipewrite(FdEngine.fds[fdId].pipe, buffer, strlen_(buffer));
}

void sysRead(int fd, char *buffer) {

  int fdId = findProcessFd(fd);

  if (fdId < 0)
    ncPrint(MSG_ERROR_READING);
  else if (FdEngine.fds[fdId].id == STDIN)
    getBufferChar(buffer);
  else
    piperead(FdEngine.fds[fdId].pipe, buffer, 1);
}

void close(struct file *fd) { pipeclose(fd->pipe, fd->writable); }
