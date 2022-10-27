
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
  Spinlock lock;
  char data[PIPESIZE];
  Process blocked[MAX_BLOCKED]; // probably better to be a linked list
  int next;
  unsigned int nread, nwrite;
  int readopen, writeopen;
} * Pipe;

struct file {
  char type, readable, writable;
  Pipe pipe;
};

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

#endif
