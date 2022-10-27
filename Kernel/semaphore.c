#include "include/semaphores.h"
#include "include/MemoryManager.h"

/*
    Data type for a semaphore, which contains the value, name (if implemented) 
    Turn type variable, and the amount of processes that use the semaphore
*/
typedef struct SemCDT{
    unsigned int id;
    int value;
    Turn semTurn; 
    unsigned int processesCount;
    //queue
} SemCDT;

/*
    Array that keeps track of all the semaphores created
    
*/
static Semaphore semaphores[MAX_SEM] = {NULL};
/*
    Counter of the semaphores running
*/
static unsigned int semaphoresCount = 0;


static int findSemaphore(Semaphore semaphore){

    if(semaphore != NULL && semaphores[semaphore->id] == NULL)
        return SEM_OK;

    return SEM_NOT_EXISTS;
}

int semWait(Semaphore semaphore){

    if(findSemaphore(semaphore) == SEM_NOT_EXISTS)
        return SEM_NOT_EXISTS;
    
    try_lock(&(semaphore->semTurn));

    (semaphore->value)--;

    unlock(&(semaphore->semTurn));
    return SEM_OK;
}

int semSignal(Semaphore semaphore){

    if(semaphore == NULL || findSemaphore(semaphore) == SEM_NOT_EXISTS)
        return SEM_NOT_EXISTS;

    try_lock(&(semaphore->semTurn));

    (semaphore->value)++;

    //Wake up one process blocked by the wait

    unlock(&(semaphore->semTurn));
    return SEM_OK;
}

int semCreate(Semaphore * semaphore){
    
    //In case that we have reached the limit for semaphores
    if(semaphoresCount == MAX_SEM)
        return SEM_SIZE_LIMIT_REACHED;

    //Alloc for the semaphore
    *semaphore = allocMemory(sizeof(SemCDT));
    for(int i = 0; i < MAX_SEM; i++){
        if(semaphores[i] == NULL){
            semaphores[i] = *semaphore;
            (*semaphore)->id = i;
            (*semaphore)->processesCount = 0;
            (*semaphore)->value = 1;
            (*semaphore)->semTurn = 0;
            semaphoresCount++;
            return (*semaphore)->id = i;
        }
    }
    return SEM_ERROR;
}

Semaphore semOpen(int id){
    
    if(id < 0 || id >= MAX_SEM)
        return NULL;
    
    if(semaphores[id] != NULL){
        //There's one more process using this semaphore
        (semaphores[id]->processesCount)++;
        //Return the pointer to the semaphore
        return semaphores[id];
    }
    return NULL;
}

int semClose(Semaphore semaphore){
    if(findSemaphore(semaphore) != SEM_OK)
        return SEM_NOT_EXISTS;

    // There's one less process using this semaphore
    (semaphore->processesCount)--;

    // In case the last process closed the semaphore, it will be destroid
    if(semaphore->processesCount == 0){
        int id = semaphore->id;
        freeMemory(semaphore);
        semaphores[id] = NULL;
    }

    return SEM_OK;
}


int initializeSemaphoreSystem(){

}