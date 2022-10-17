// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "include/MemoryManager.h"
#include "include/schedluerEngine.h"
#include <naiveConsole.h>
#include <interrupts.h>



void initialiseContextSchedluerEngine() {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        procesos[i].flagRunning = 0;
        procesos[i].flagPaused = 1;
        }
}

int toSwitch() {
    // Aca implementamos la manera mas basica de switchear los procesos 
    // en base a los timer ticks. El mismo numbero de timer ticks para
    // todos los procesos. En si aca se podria crear una tabla adicional
    // con los timer tick que cada comando deberia esperar para ser
    //switcheado
    
    ticks++;

    if(ticks == TICKS) {
        ticks = 0;
        return 1;
    }

    return 0;
}

void switchContext(long * contextHolder, int * contextOwner) {
    
    if(!toSwitch()) return;

    if(processesRunning == 0) return;
    pushContext(contextHolder, *contextOwner);
    *contextOwner = nextProcess(contextOwner);
    popContext(contextHolder, *contextOwner);
    return;
}



char  nextProcess(int * contextOwner ) {
    //Aca planteamos el algoritmo de schedluing, en si implementamos el mas simple
    //el Round Robin. La clave del while este es que siempre voy a a tener un proceso
    //corriendo, la shell (funciona como nuestro proceso idle)

    int  next =  (*contextOwner + 1) % MAX_PROCESSES;
    while(!(procesos[next].flagRunning && !procesos[next].flagPaused)) {
        next =  (next +  1) % MAX_PROCESSES;
    }
    return next;
}

static void pushContext(long * contextHolder, int  contextOwner){
    for (int i = 0; i < 18; i++)
        procesos[contextOwner].context.registers[i] = contextHolder[i];
    
}

static void popContext(long * contextHolder, int  contextOwner){
    for (int i = 0; i < 18; i++)
       contextHolder[i] = procesos[contextOwner].context.registers[i];
}

int exitProces(long * contextHolder,int * contextOwner){
    procesos[*contextOwner].flagRunning = 0;
    procesos[*contextOwner].flagPaused = 1;
    processesRunning -= 1;
    *contextOwner = nextProcess(contextOwner);
    popContext(contextHolder, *contextOwner);
    return processesRunning;

}
int killProcess(int pid){
    if(procesos[pid].flagRunning){
        procesos[pid].flagRunning = 0;
        processesRunning -= 1;   
    }
    return processesRunning;
}

int pauseProces(int pid){
    if(!procesos[pid].flagPaused){
        procesos[pid].flagPaused = 1;
        processesPaused += 1;   
    }
    
    return processesRunning;
}
int reloadProcess(int pid){
    if(procesos[pid].flagPaused){
        procesos[pid].flagPaused = 0;
        processesPaused -= 1;
    }
    return processesRunning;
}
void loadFirstContext(long * contextHolder){
    if (processesRunning == MAX_PROCESSES) return;
    pushContext(contextHolder, processesRunning);
    procesos[processesRunning].context.registers[RSP] = (long)(procesos[processesRunning].stackFrame + MAX_STACK -1);
    procesos[processesRunning].flagRunning = 1;
    procesos[processesRunning].flagPaused = 0;
    /*
        Lo que hago en la siguiente linea es tomar el valor guardado en RDI para tomar su primer parametro
        el cual por la firma de cada una de las funciones es el FD para luego utilizarlo
    */
    procesos[processesRunning].fileDescriptor = procesos[processesRunning].context.registers[RDI];
    popContext(contextHolder, processesRunning);
    processesRunning += 1;
}
int getFD(int contexOwner){
    return procesos[contexOwner].fileDescriptor;
}
