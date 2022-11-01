
#ifndef FILE_DESCRIPTOR_MANAGER_H
#define FILE_DESCRIPTOR_MANAGER_H

// SCHEDULER.h
// /* closes a file descriptor as it won't be used */
// void close(struct file * fd);
//
// /* write to a file descriptor a set number of bytes */
// int write(struct file * fd, char * addr, int n);
//
// /* read from a file descriptor a set number of bytes */
// int read(struct file * fd, char * addr, int n);
#include "semaphores.h"

#define MAX_PIPE_NUMBER 200
#define MAX_FD_COUNT 500

#define MAX_BLOCKED 20
#define PIPESIZE 512

#define STDOUT 0
#define STDIN 1

typedef struct process {
  char type, pid;
} Process;

typedef struct pipe {

  // mantengo un lock para manejar escrituras/lecturas
  // al pipe de manera concurrente.
  Semaphore lock;

  // mantengo un arreglo circular como buffer del pipe
  //(y los indices de posicion a leer por escritor y lector)
  char data[PIPESIZE];
  unsigned int nread, nwrite;

  // mantengo la lista de los procesos bloqueados esperando
  // a leer/escribir al pipe como arreglo circular
  Process blocked[MAX_BLOCKED];
  int next;

  // no son imperativos, pero nos permiten un mejor manejo del pipe
  // al momento de leer/escribir
  int readopen;
  int writeopen;
} * Pipe;

typedef struct file {
  char type, readable, writable;
  int id;
  Pipe pipe;
} * File;

/* mantengo una lista con todos los fd del sistema */
struct fdEngine {
  struct file fds[MAX_FD_COUNT];
  int next;
};

/* devuelve el siguiente fd de la tabla de fd globales */
File allocFileDescriptor();

void initFdManager();

/* proceso escribe a su fd de out un buffer */
void sysWrite(int fd, char *buffer);

/* proceso lee a su fd de out un buffer */
void sysRead(int fd, char *buffer);

/* reemplaza el oldfd de los fd de proceso con pid
 * recibido por el newfd */
void dup2(int pid, int oldfd, int newfd);

#endif
