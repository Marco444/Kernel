#include "include/fileDescriptorManager.h"
#include "include/keyBoard.h"
#include "include/lib.h"
#include "include/list.h"
#include "include/naiveConsole.h"
#include "include/pipeEngine.h"
#include "include/schedluerEngine.h"

struct fdEngine FdEngine;

/* Tenemos el fd que maneja el proceso, el que hace referencia al indice en su
 * tabla de fd abiertos y luego el fd que determina el File en el fdEngine. */

int verifyFd(int fd) { return fd >= 0 && fd < MAX_FD_PROCESS; }

// oldfd will be that returned to the userland,
// the INDEX in the fds table of the pcb,
// whilst the newfd will be that returned by pipe
// the INDEX in the FdEngine

int dup2(int oldfd, int newfd) {

  if (!verifyFd(oldfd))
    return -1;

  int *fdIds = currentProcessFds();

  fdIds[oldfd] = FdEngine.fds[newfd].id;

  return 0;
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
  if (!verifyFd(fd))
    return -1;

  int *fdIds = currentProcessFds();
  return fdIds[fd];
}

int sysWrite(int fd, char *buffer) {
  int fdId = findProcessFd(fd);

  if (fdId < 0)
    return -1;

  if (FdEngine.fds[fdId].id != STDOUT) {
    ncPrint("my output fd is: ");
    ncPrintDec(FdEngine.fds[fdId].id);
    ncNewline();
  }

  ncPrint(buffer);
  // else
  //   pipewrite(FdEngine.fds[fdId].pipe, buffer, strlen_(buffer));

  return 0;
}

int sysRead(int fd, char *buffer) {

  int fdId = findProcessFd(fd);

  if (fdId < 0)
    return -1;

  if (FdEngine.fds[fdId].id != STDIN)
    ;

  getBufferChar(buffer);
  // else
  //   piperead(FdEngine.fds[fdId].pipe, buffer, 1);

  return 0;
}

void close(struct file *fd) { pipeclose(fd->pipe, fd->writable); }
