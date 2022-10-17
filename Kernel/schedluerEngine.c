// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "include/schedluerEngine.h"
#include <naiveConsole.h>
#include <interrupts.h>
#include <MemoryManager.h>
//extern void pushFirstStack(char * stackPointer,long instructionPointer);
void initialiseContextSchedluerEngine()
{
    procesos = allocMemory(sizeof(Process) * MAX_PROCESSES);
    for (int i = 0; i < MAX_PROCESSES; i++)
    {
        procesos[i].flagRunning = 0;
        procesos[i].flagPaused = 1;
    }

}

int toSwitch()
{
    // Aca implementamos la manera mas basica de switchear los procesos
    // en base a los timer ticks. El mismo numbero de timer ticks para
    // todos los procesos. En si aca se podria crear una tabla adicional
    // con los timer tick que cada comando deberia esperar para ser
    // switcheado

    ticks++;

    if (ticks == TICKS)
    {
        ticks = 0;
        return 1;
    }

    return 0;
}

long switchContext(long rsp)
{
    if (!toSwitch())
       return procesos[contextOwner].context.registers[RSP];

    if (processesRunning == 0)
        return procesos[contextOwner].context.registers[RSP];
    if (contextOwner == -1 ){
        contextOwner = 0;
        return procesos[contextOwner].context.registers[RSP];
    }
    procesos[contextOwner].context.registers[RSP] = rsp;
    //pushContext(contextHolder, *contextOwner);
     contextOwner = nextProcess(contextOwner);
    //popContext(contextHolder, *contextOwner);
    return procesos[contextOwner].context.registers[RSP];
    
}

void updateRsp(long rsp){
    procesos[processesRunning - 1].context.registers[RSP] = rsp;
}

char  nextProcess()
{
    // Aca planteamos el algoritmo de schedluing, en si implementamos el mas simple
    // el Round Robin. La clave del while este es que siempre voy a a tener un proceso
    // corriendo, la shell (funciona como nuestro proceso idle)

    int next = (contextOwner + 1) % MAX_PROCESSES;
    while (!(procesos[next].flagRunning && !procesos[next].flagPaused))
    {
        next = (next + 1) % MAX_PROCESSES;
    }
    return next;
}

static void pushContext(long *contextHolder)
{
    for (int i = 0; i < 18; i++)
        procesos[contextOwner].context.registers[i] = contextHolder[i];
}

static void popContext(long *contextHolder)
{
    for (int i = 0; i < 18; i++)
        contextHolder[i] = procesos[contextOwner].context.registers[i];
}

int exitProces(long *contextHolder)
{
    procesos[contextOwner].flagRunning = 0;
    procesos[contextOwner].flagPaused = 1;
    processesRunning -= 1;
    contextOwner = nextProcess(contextOwner);
    popContext(contextHolder);
    return processesRunning;
}
int killProcess(int pid)
{
    if (procesos[pid].flagRunning)
    {
        procesos[pid].flagRunning = 0;
        processesRunning -= 1;
    }
    return processesRunning;
}

int pauseProces(int pid)
{
    if (!procesos[pid].flagPaused)
    {
        procesos[pid].flagPaused = 1;
        processesPaused += 1;
    }

    return processesRunning;
}
int reloadProcess(int pid)
{
    if (procesos[pid].flagPaused)
    {
        procesos[pid].flagPaused = 0;
        processesPaused -= 1;
    }
    return processesRunning;
}

/*

*/



long loadFirstContext()
{
 
    
    if (processesRunning == MAX_PROCESSES)
        return;


    /* Esto se podria eliminar creo*/
    //pushContext(contextHolder, processesRunning);

    /*Armo la pcb faltaria TODO agregar cosas*/

    // Aca deberia hacer una alloc pero lo dejo para luego
    procesos[processesRunning].stackFrame = stacks[0];
    procesos[processesRunning].context.registers[RSP] = (long)(procesos[processesRunning].stackFrame + MAX_STACK - 1);
    long toReturn = procesos[processesRunning].context.registers[RSP];
    procesos[processesRunning].flagRunning = 1;
    procesos[processesRunning].flagPaused = 0;
    
    /*Esto tambien se podria eliminar creo*/
    //procesos[processesRunning].fileDescriptor = procesos[processesRunning].context.registers[RDI];
    //popContext(contextHolder, processesRunning);

    processesRunning += 1;
    return procesos[processesRunning-1].context.registers[RSP];
}
int getFD(int contextOwner)
{
    return procesos[contextOwner].fileDescriptor;
}
