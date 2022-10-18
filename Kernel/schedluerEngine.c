// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "include/schedluerEngine.h"
#include <naiveConsole.h>
#include <interrupts.h>
#include <MemoryManager.h>
//Incluimos la funcion que proviene de assembler para loadear el primer contexto
extern long loadContext(int window,char ** argV,int argC,long rsp,void * funcPointer);
void initialiseContextSchedluerEngine()
{
    
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

    
    if (processesRunning == 0)
        return rsp;
    if (contextOwner == -1 ){
        contextOwner = 0;
        return procesos[contextOwner].context.registers[RSP];
    }

    procesos[contextOwner].context.registers[RSP] = rsp;
     contextOwner = nextProcess(contextOwner);
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
    if (contextOwner == 0 && procesos[processesRunning - 1].flagRunning )
    {
        return processesRunning - 1;
    }else{
        return 0;
    }
    

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
    Lo que hace esta funcion es buscar memoria para el stack y cargarlo dicho stack
*/
void loadFirstContext(void * funcPointer,int window, int argC,char **argv)
{
 
    
    if (processesRunning == MAX_PROCESSES)
        return;
    // Aca deberia hacer una alloc pero lo dejo para luego
    procesos[processesRunning].stackFrame = stacks[processesRunning];
    procesos[processesRunning].context.registers[RSP] = (long)(procesos[processesRunning].stackFrame + MAX_STACK - 1);
    procesos[processesRunning].flagRunning = 1;
    procesos[processesRunning].flagPaused = 0;
    procesos[processesRunning].context.registers[RSP] = loadContext(window,argC,argv,procesos[processesRunning].context.registers[RSP],funcPointer);
    processesRunning += 1;
}
int getFD(int contextOwner)
{
    return procesos[contextOwner].fileDescriptor;
}
