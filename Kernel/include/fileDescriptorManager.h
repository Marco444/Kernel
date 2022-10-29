
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

#define MAX_FD_COUNT 252

#define MAX_BLOCKED 20
#define PIPESIZE 512
#define MAX_PIPE_NUMBER 100

typedef struct process {
  char type, pid;
} Process;

typedef struct spinlock {

} * Spinlock;

typedef struct pipe {

  // mantengo un lock para manejar escrituras/lecturas
  // al pipe de manera concurrente.
  Spinlock lock;

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
  Pipe pipe;
} * File;

#endif
