#include "include/semaphores.h"
#include "include/MemoryManager.h"
#include "include/schedluerEngine.h"

/*
    Data type for a semaphore, which contains the value, name (if implemented) 
    Turn type variable, and the amount of processes that use the semaphore
*/
typedef struct SemCDT{
    unsigned int id;
    int value;
    Turn semTurn; 
    unsigned int processesCount;
    unsigned int processesWait[MAX_PROCESSES];
    unsigned int firstPosWait;
    unsigned int lastPosWait;
    short int emptyWait;
} SemCDT;

/*
    Array that keeps track of all the semaphores created
*/
static Semaphore semaphores[MAX_SEM] = {NULL};
/*
    Counter of the semaphores running
*/
static unsigned int semaphoresCount = 0;
/*
    Check if Semaphore is valid?
*/
static int findSemaphore(Semaphore semaphore){

    if(semaphore != NULL && semaphores[semaphore->id] == NULL)
        return SEM_OK;

    return SEM_NOT_EXISTS;
}

static void sleepProcess(Semaphore semaphore){
        semaphore->processesWait[semaphore->lastPosWait] = currentPid();
    
        semaphore->lastPosWait = (semaphore->lastPosWait + 1) % MAX_PROCESSES;
        
        semaphore->emptyWait = 0;

        blockProcess(currentPid());
}

int semWait(Semaphore semaphore){

    if(findSemaphore(semaphore) == SEM_NOT_EXISTS)
        return SEM_NOT_EXISTS;
    
    tryLock(&(semaphore->semTurn));

    (semaphore->value)--;

    if(semaphore->value < 0)
        sleepProcess(semaphore);

    unlock(&(semaphore->semTurn));
    return SEM_OK;
}

int semSignal(Semaphore semaphore){

    if(semaphore == NULL || findSemaphore(semaphore) == SEM_NOT_EXISTS)
        return SEM_NOT_EXISTS;

    tryLock(&(semaphore->semTurn));

    (semaphore->value)++;

    // If the value is greater than 0
    // Wake up one process blocked by the wait (in case the queue is not empty)
    if(semaphore->value > 0 && !semaphore->emptyWait){
        // Wake up process in the queue
        unblockProcess(semaphore->processesWait[semaphore->firstPosWait]);
        // Move to the next in the queue
        (semaphore->firstPosWait) = ((semaphore->firstPosWait) + 1) % MAX_PROCESSES;
        // In case we took the last one
        if(semaphore->firstPosWait == semaphore->lastPosWait)
            semaphore->emptyWait = 1;
    }
    unlock(&(semaphore->semTurn));
    return SEM_OK;
}

static int semCreate(Semaphore * semaphore){
    
    //In case that we have reached the limit for semaphores
    if(semaphoresCount >= MAX_SEM - 1)
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
            (*semaphore)->lastPosWait = 0;
            (*semaphore)->firstPosWait = 0;
            (*semaphore)->emptyWait = 1;
            semaphoresCount++;
            return SEM_OK;
        }
    }
    return SEM_ERROR;
}

Semaphore semOpen(int id){
    // We check that the id is valid
    if(id < 0 || id >= MAX_SEM)
        return NULL;
    
    // In case the semaphore already exists, we return the one that exists
    if(semaphores[id] != NULL){
        // In case we have reached the limit of processes using this semaphore
        if(semaphores[id]->processesCount == MAX_PROCESSES)
            return NULL;
        //There's one more process using this semaphore
        (semaphores[id]->processesCount)++;
        //Return the pointer to the semaphore
        return semaphores[id];
    }
    
    // Otherwise we'll create it
    Semaphore toReturn;
    
    if(semCreate(&toReturn) != SEM_OK)
        return NULL;
    
    return toReturn;
}

int semClose(Semaphore semaphore){
    
    // We check that the semahpore given is valid
    if(findSemaphore(semaphore) != SEM_OK)
        return SEM_NOT_EXISTS;

    // There's one less process using this semaphore
    (semaphore->processesCount)--;

    // In case the last process closed the semaphore, it will be destroid
    if(semaphore->processesCount == 0){
        int id = semaphore->id;
        freeMemory(semaphore);
        semaphores[id] = NULL;
        semaphoresCount--;
    }

    return SEM_OK;
}


int initializeSemaphoreSystem(){

}