
#ifndef PIPE_ENGINE
#define PIPE_ENGINE

#include "MemoryManager.h"
#include "naiveConsole.h"

#define PIPESIZE 512
#define MAX_PIPE_NUMBER 100

#define FD_PIPE 11
#define MAX_BLOCKED 20

#define READER 1
#define WRITER 0

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

/* levanta a los READER/WRITER del Pipe p segun el type */
void wakeup(Pipe p, char type);

/* manda a la lista de bloqueados del scheduler y los bloqueados del pipe al
 * proceso que lo llama, almacenando si es un lector o un escritor */
void sleep(Pipe p, char type);

/* se crea un pipe a partir de dos punteros a fd, alocando memoria para ellos
 * tambien. */
int pipe(struct file **f0, struct file **f1);

/* un proceso que tiene el pipe le escribe una cantidad de bytes */
int pipewrite(Pipe p, char *addr, int n);

/* un proceso que tiene el pipe lee una cantidad de bytes */
int piperead(Pipe p, char *addr, int n);

/* imprime a pantalla el estado de todos los pipes del sistema */
void pipesDump();

/* this function serves to tell the kernel that the pipe is no longer being
 * used either in write or read, thus allowing the system to free the memory*/
void pipeclose(Pipe p, int writable);

File *allocFileDescriptor();

#endif
