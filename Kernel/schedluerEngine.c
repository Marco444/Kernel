
#include "include/schedluerEngine.h"
#include <naiveConsole.h>
#include <interrupts.h>

static int ticks = 0;

void initialiseContextSchedluerEngine() {
    for (int i = 0; i < MAX_PROCESSES; i++) procesos[i].flagRunning = 0;
}

void switchContext(long * contextHolder, char * contextOwner) {
    
    ticks++;
    if(ticks % TICKS != 0) 
        return;

    if(processesRunning == 0) return;
    pushContext(contextHolder, *contextOwner);
    *contextOwner = nextProcess(contextOwner);
    popContext(contextHolder, *contextOwner);
    return;
}

char  nextProcess(char * contextOwner ) {
    //Aca planteamos el algoritmo de schedluing, en si implementamos el mas simple
    //el Round Robin. La clave del while este es que siempre voy a a tener un proceso
    //corriendo, la shell (funciona como nuestro proceso idle)

    char  next =  (*contextOwner + 1) % processesRunning;
    while(!procesos[next].flagRunning) {
        next =  ((*contextOwner) + 1) % processesRunning;
    }
    return next;
}

static void pushContext(long * contextHolder, char  contextOwner){
    for (int i = 0; i < 18; i++)
        procesos[(int)contextOwner].context.registers[i] = contextHolder[i];
    
}
static void popContext(long * contextHolder, char  contextOwner){
    for (int i = 0; i < 18; i++)
       contextHolder[i] = procesos[(int)contextOwner].context.registers[i];
}

void exitProces(long * contextHolder,char * contextOwner){
    procesos[(int)(*contextOwner)].flagRunning = 0;
    processesRunning -= 1;
    *contextOwner = nextProcess(contextOwner);
    popContext(contextHolder, *contextOwner);
}

void loadFirstContext(long * contextHolder){
    if (processesRunning == MAX_PROCESSES) return;
    pushContext(contextHolder, processesRunning);
    procesos[processesRunning].context.registers[RSP] = (procesos[processesRunning].stackFrame + MAX_STACK -1);
    procesos[processesRunning].flagRunning = 1;
    popContext(contextHolder, processesRunning);
    processesRunning += 1;
}