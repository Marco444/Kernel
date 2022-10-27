#include "include/semaphores.h"

/*
    Data type for a semaphore, which contains the value, name (if implemented) 
    Turn type variable, and the amount of processes that use the semaphore
*/
typedef struct {
    unsigned int id;
    int value;
    //char * name;
    Turn semTurn; 
    unsigned int processesCount;
    //queue
} SemCDT;

/* 
    Semaphore Type is a point to the semaphore data type, which provides a layer of abstraction 
    so this data is not accesisable 
*/
typedef SemCDT * Semaphore;

/*
    Array that keeps track of all the semaphores created
    Counter of the semaphores running
*/
static Semaphore semaphores[MAX_SEM] = {NULL};
static unsigned int semaphoresCount = 0;


static int findSemaphore(Semaphore semaphore){

    if(semaphore == NULL || semaphores[semaphore->id] == NULL)
        return SEM_OK;

    return SEM_NOT_EXISTS;
}

int semWait(Semaphore semaphore){

    if(findSemaphore(semaphore) == SEM_NOT_EXISTS)
        return SEM_NOT_EXISTS;
    
    try_lock(&(semaphore->semTurn));

    

    unlock(&(semaphore->semTurn));
    return SEM_OK;
}

int semSignal(Semaphore semaphore){

    if(semaphore == NULL || findSemaphore(semaphore) == SEM_NOT_EXISTS)
        return SEM_NOT_EXISTS;

    try_lock(&(semaphore->semTurn));

    

    unlock(&(semaphore->semTurn));

}

int semOpen(Semaphore * semaphore){

}

int semClose(Semaphore semaphore){
    
}
