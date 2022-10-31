#include "include/fileDescriptorManager.h"

// SCHEDULER.C
// void close(struct file * fd) {
//   pipeclose(fd->pipe, fd->writable);
// }
//
// int write(struct file * fd, char * addr, int n) {
//   return pipewrite(fd->pipe, addr, n);
// }
//
// int read(struct file * fd, char * addr, int n) {
//   return piperead(fd->pipe, addr, n);
// }
