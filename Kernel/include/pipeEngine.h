
#ifndef PIPE_ENGINE
#define PIPE_ENGINE

#include "MemoryManager.h"
#include "fileDescriptorManager.h"
#include "naiveConsole.h"

#define PIPESIZE 512
#define MAX_PIPE_NUMBER 100

#define FD_PIPE 11

#define READER 1
#define WRITER 0

/* levanta a los READER/WRITER del Pipe p segun el type */
void wakeup(Pipe p, char type);

/* manda a la lista de bloqueados del scheduler y los bloqueados del pipe al
 * proceso que lo llama, almacenando si es un lector o un escritor */
void sleep(Pipe p, char type);

/* se crea un pipe a partir de dos punteros a fd, alocando memoria para ellos
 * tambien. */
int pipe(File *f0, File *f1);

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
