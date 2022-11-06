
#include "syscalls.h"

#define FOREGROUND 0
#define BACKGROUND 1

/* libera la memoria guardada en adress de manera dinamica */
void free(const void *address);

/* aloca memoria dinamica de tamanio size y devuelve el puntero */
void *alloc(int size);

/* imprime en pantalla el estado de la memoria dinamica del sistema */
void memorydump();

/* imprime en pantalla el estado de pipes del sistema*/
void pipesdump();

/* imprime los procesos activos en el sistema */
void psdump();

/* cambia el oldfd por el newfd en la tabla de file descriptor del proceso que
 * lo llama */
int dup2(int oldfd, int newfd);

/* crea un pipe y devuelve los fd que tiene el mismo */
int pipe(int fd[2]);

/* cierra un file descriptor */
void close(int fd);
