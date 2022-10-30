#ifndef SEMAPHORES_H
#define SEMAPHORES_H

#include <stdio.h>
#define MAX_SEM 32
#define MAX_PROCESSES 32
#define SEM_OK 0
#define SEM_NOT_EXISTS -1
#define SEM_SIZE_LIMIT_REACHED -2
#define SEM_PROCESS_LIMIT_REACHED -3
#define SEM_ERROR -4

/* 
    Semaphore Type is a pointer to the semaphore data type, which provides a 
    layer of abstraction so this data is not accesisable 
*/
typedef struct SemCDT * Semaphore;

/*
    The 'Turn' type will be used for checking and adquiring the semaphore
*/
typedef short unsigned Turn;

/*
    Assembly routines
*/
extern int tryLock(Turn turn);
extern int unlock(Turn turn);

int initializeSemaphoreSystem();

int semWait(Semaphore semaphore);

int semSignal(Semaphore semaphore);

Semaphore semOpen(int id);

int semClose(Semaphore semaphore);

int getNextAvailableSemaphore();

#endif