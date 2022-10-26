// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "include/schedluerEngine.h"
#include <naiveConsole.h>
#include <interrupts.h>
#include <MemoryManager.h>
//Incluimos la funcion que proviene de assembler para loadear el primer contexto
extern long loadContext(int window,char ** argV,int argC,long rsp,void * funcPointer);
extern void _hlt();
extern void _sti();
/*
void initialiseContextSchedluerEngine()
{
    for (int i = 0; i < MAX_PROCESSES; i++)
    {
        procesos[i].flagRunning = 0;
        procesos[i].flagPaused = 1;
    }

}
*/

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
    // Aca debemos ver si el proceso actual esta bloqueado y si lo esta lo debemos
    // pasar a la cola de bloqueados.
    if (processesRunning == 0)
        return rsp;
    if (contextOwner == -1 ){
        contextOwner = 0;
        return prioritiesReady[actualPriority].actual->data.stackPointer;
    }
    // Esto gracias al caso de que mi actual sea el primero y que le hagan un kill o un exit
    if (prioritiesReady[actualPriority].actual == NULL)
    {
        actualPriority = nextProcess();
        prioritiesReady[actualPriority].actual ->data.quantum--;
        return prioritiesReady[actualPriority].actual ->data.stackPointer;
    }
    
    
    prioritiesReady[actualPriority].actual ->data.stackPointer = rsp;
    if (prioritiesReady[actualPriority].actual->data.state == BLOCK){
        sendToWaitingList();
        actualPriority = nextProcess();
    }
    else if (!prioritiesReady[actualPriority].actual ->data.quantum){
        prioritiesReady[actualPriority].actual ->data.quantum = prioritiesQuatums[actualPriority];
        actualPriority = nextProcess();
    }
    prioritiesReady[actualPriority].actual ->data.quantum--;
    return prioritiesReady[actualPriority].actual ->data.stackPointer;
    
}
sendToWaitingList(){
    // Debo pensar como eliminarlo de la lista y luego ponerlo al inicio de la otra lista

    //En el caso que este en el medio
    // Esta mal creo porque si lo saco dsp si lo saco el actual va a ser null
    if (prioritiesReady[actualPriority].actual->next && prioritiesReady[actualPriority].actual->before )
    {
       prioritiesReady[actualPriority].actual->next->before = prioritiesReady[actualPriority].actual->before;
       prioritiesReady[actualPriority].actual->before->next =  prioritiesReady[actualPriority].actual->next;
       prioritiesReady[actualPriority].actual = prioritiesReady[actualPriority].actual->next;
    }else if(!prioritiesReady[actualPriority].actual->next && !prioritiesReady[actualPriority].actual->before){
        prioritiesReady[actualPriority].actual = NULL;
        prioritiesReady[actualPriority].first = NULL;
    }else if(!prioritiesReady[actualPriority].actual->next){
        prioritiesReady[actualPriority].actual->before->next = NULL;
    }else{
        prioritiesReady[actualPriority].first = prioritiesReady[actualPriority].actual->next;
        prioritiesReady[actualPriority].actual = prioritiesReady[actualPriority].actual->next;

    }
}
char nextProcess()
{
    // Aca planteamos el algoritmo de schedluing, en si implementamos el mas simple
    // el Round Robin. La clave del while este es que siempre voy a a tener un proceso
    // corriendo, la shell (funciona como nuestro proceso idle)

    int next = (actualPriority) % CANT_PRIORITIES;
    if(prioritiesReady[next].actual != NULL)
        prioritiesReady[next].actual = prioritiesReady[next].actual->next;
    while(!prioritiesReady[next].actual || !prioritiesReady[next].actual->data.flagRunning)
    {
        if (!prioritiesReady[next].actual){
            prioritiesReady[next].actual = prioritiesReady[next].first;
            next = (next + 1) % CANT_PRIORITIES;
        }
        else
            prioritiesReady[next].actual = prioritiesReady[next].actual->next;
    }
    return next;
}

long exitProces()
{
    killProcess(prioritiesReady[actualPriority].actual->data.pid);
    return  switchContext(0);
}
int getProcesses(){
    return processesRunning;
}
int killProcess(int pid)
{
    for (int i = 0; i < CANT_PRIORITIES; i++)
    {
        Node * current = prioritiesReady[i].first;
        while (current != NULL)
        {
           if (current->data.pid == pid)
           {
             processesRunning -= 1;
            
             current->before->next = current->next;
             current->next->before = current->before->next;
             freeMemory(current);
             return processesRunning;
           }
           current = current->next;
        }
    }
    return processesRunning;
}

int pauseProces(int pid)
{
    if (!prioritiesReady[actualPriority].first->data.flagPaused)
    {
        prioritiesReady[actualPriority].first->data.flagPaused = 1;
        processesPaused += 1;
    }

    return processesRunning;
}
int reloadProcess(int pid)
{
    if (prioritiesReady[actualPriority].first->data.flagPaused)
    {
        prioritiesReady[actualPriority].first->data.flagPaused = 0;
        processesPaused -= 1;
    }
    return processesRunning;
}
int waitpid(int pid){
    if (waitingProcess == NULL)
    {
        waitingProcess = allocMemory(sizeof(waitingProcessNode));
    }else{
        Node * aux = allocMemory(sizeof(waitingProcessNode));
        aux->next = waitingProcess;
        waitingProcess = aux;
    }
    
    waitingProcess->processNode = prioritiesReady[actualPriority].actual;
    waitingProcess->waitingPid = pid;
   
    
}

/*
    Lo que hace esta funcion es buscar memoria para el stack y cargarlo dicho stack
*/
int loadFirstContext(void * funcPointer,int window, int argC,char **argv,int backGround)
{
    
    int newProcessPriority = 0;
    // Lo hago de esta manera para que la shell tenga una prioridad mayor
    if (processesRunning)
        newProcessPriority = DEFAULT_PRIORITY;
    
    addNewProcess(newProcessPriority);
    //TODO esto se podria poner en ADT de la lista para que la misma maneje estas cuestiones
    
    // Aca deberia hacer una alloc pero lo dejo para luego
    prioritiesReady[newProcessPriority].first->data.stackPointer = stacks[processesRunning];
    prioritiesReady[newProcessPriority].first->data.stackPointer +=   MAX_STACK - 1;
    prioritiesReady[newProcessPriority].first->data.flagRunning = 1;
    prioritiesReady[newProcessPriority].first->data.flagPaused = 0;
    prioritiesReady[newProcessPriority].first->data.pid = nextProcessPid++;
    prioritiesReady[newProcessPriority].first -> data.quantum = prioritiesQuatums[newProcessPriority];
    if(!backGround)
        waitpid
    prioritiesReady[newProcessPriority].first->data.stackPointer = loadContext(window,argC,argv,prioritiesReady[newProcessPriority].first->data.stackPointer,funcPointer);
    processesRunning += 1;
    return prioritiesReady[newProcessPriority].first->data.pid;
}

void addNewProcess(int newProcessPriority){
      if (prioritiesReady[newProcessPriority].first == NULL)
    {
        prioritiesReady[newProcessPriority].first = allocMemory(sizeof(Node));
        prioritiesReady[newProcessPriority].first->before = NULL;
        prioritiesReady[newProcessPriority].actual = prioritiesReady[newProcessPriority].first;
    }else{
        Node * aux = prioritiesReady[newProcessPriority].first;
        prioritiesReady[newProcessPriority].first = allocMemory(sizeof(Node));
        aux->before = prioritiesReady[newProcessPriority].first;
        prioritiesReady[newProcessPriority].first->next = aux;
    }
}

int getFD(int contextOwner)
{
    return prioritiesReady[actualPriority].first->data.fileDescriptor;
}
