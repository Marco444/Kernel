#ifndef SEMAPHORES_H
#define SEMAPHORES_H

#include <stdio.h>
#define MAX_SEM 32
#define SEM_OK 1
#define SEM_NOT_EXISTS -1


/*
    Assembly routines
*/
extern int try_lock(Turn turn);
extern int unlock(Turn turn);

/*
    The 'Turn' type will be used for checking and adquiring the semaphore
*/
typedef short unsigned Turn;



int semWait(Semaphore semaphore);

int semSignal(Semaphore semaphore);

int semOpen(Semaphore * semaphore);

int semClose(Semaphore semaphore);




// /*
//     Mutex Implementation
// */

// typedef unsigned int Mutex;

// int mutexInitialize(Mutex * mutex);

// int mutexTerminate(Mutex * mutex);

// int mutexOpen(Mutex mutex);

// int mutexClose(Mutex mutex);


#endif