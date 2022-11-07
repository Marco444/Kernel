// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
#include "../include/fileDescriptorManager.h"

#include "../include/keyBoard.h"
#include "../include/lib.h"
#include "../include/list.h"
#include "../include/naiveConsole.h"
#include "../include/pipeEngine.h"
#include "../include/schedluerEngine.h"
#include "../include/semaphores.h"

struct fdEngine FdEngine;

int verifyFd(int fd) { return fd >= 0 && fd < MAX_FD_PROCESS; }

int dup2(int oldfd, int newfd) {
  if (!verifyFd(oldfd)) return -1;

  int *fdIds = currentProcessFds();

  fdIds[oldfd] = newfd;

  return 0;
}

File allocFileDescriptor() {
  int startIdx = FdEngine.next;

  do {
    FdEngine.next = (FdEngine.next + 1) % MAX_FD_COUNT;
  } while ((startIdx != FdEngine.next && FdEngine.fds[FdEngine.next].busy) ||
           FdEngine.next == STDIN || FdEngine.next == STDOUT);

  if (FdEngine.fds[FdEngine.next].busy) return NULL;

  FdEngine.fds[FdEngine.next].busy = 1;

  return &FdEngine.fds[FdEngine.next];
}

void initFdManager() {
  for (int i = 0; i < MAX_FD_COUNT; i++) {
    FdEngine.fds[i].id = i;
    FdEngine.fds[i].busy = 0;
  }

  initKeyboard();

  initPipeEngine();

  FdEngine.next = 2;
}

int findProcessFd(int fd) {
  if (!verifyFd(fd)) return -1;

  int *fdIds = currentProcessFds();
  return fdIds[fd];
}

int sysWrite(int fd, char *buffer) { return sysWriteFormat(fd, buffer, WHITE); }

int sysWriteFormat(int fd, char *buffer, char format) {
  int fdId = findProcessFd(fd);

  if (fdId < 0) return -1;

  if (fdId == STDOUT)
    ncPrintFormat(buffer, format);
  else
    pipewrite(FdEngine.fds[fdId].pipe, buffer, strlen_(buffer));

  return 0;
}

int sysRead(int fd, char *buffer) {
  int fdId = findProcessFd(fd);

  if (fdId < 0) return -1;

  if (fdId == STDIN)
    getBufferChar(buffer);
  else
    piperead(FdEngine.fds[fdId].pipe, buffer, 1);

  return 0;
}

void close(int fd) {
  int fdId = findProcessFd(fd);
  if (fdId == -1 || fdId == STDIN || fdId == STDOUT) return;
  FdEngine.fds[fdId].busy = 0;
  pipeclose(FdEngine.fds[fdId].pipe, FdEngine.fds[fdId].writable);
}
