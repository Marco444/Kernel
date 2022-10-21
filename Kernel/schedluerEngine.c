// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "include/schedluerEngine.h"
#include <MemoryManager.h>
#include <interrupts.h>
#include <naiveConsole.h>
// Incluimos la funcion que proviene de assembler para loadear el primer
// contexto
extern long loadContext(int window, char **argV, int argC, long rsp,
                        void *funcPointer);
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

int toSwitch() {
  // Aca implementamos la manera mas basica de switchear los procesos
  // en base a los timer ticks. El mismo numbero de timer ticks para
  // todos los procesos. En si aca se podria crear una tabla adicional
  // con los timer tick que cada comando deberia esperar para ser
  // switcheado
  ticks++;

  if (ticks == TICKS) {
    ticks = 0;
    return 1;
  }

  return 0;
}

long switchContext(long rsp) {

  if (processesRunning == 0)
    return rsp;
  if (contextOwner == -1) {
    contextOwner = 0;
    return priorities[actualPriority].actual->data.stackPointer;
  }
  // Esto gracias al caso de que mi actual sea el primero y que le hagan un kill
  // o un exit
  if (priorities[actualPriority].actual == NULL) {
    actualPriority = nextProcess();
    priorities[actualPriority].actual->data.quantum--;
    return priorities[actualPriority].actual->data.stackPointer;
  }

  priorities[actualPriority].actual->data.stackPointer = rsp;
  if (!priorities[actualPriority].actual->data.quantum) {
    priorities[actualPriority].actual->data.quantum =
        prioritiesQuatums[actualPriority];
    actualPriority = nextProcess();
  }
  priorities[actualPriority].actual->data.quantum--;
  return priorities[actualPriority].actual->data.stackPointer;
}
char nextProcess() {
  // Aca planteamos el algoritmo de schedluing, en si implementamos el mas
  // simple el Round Robin. La clave del while este es que siempre voy a a tener
  // un proceso corriendo, la shell (funciona como nuestro proceso idle)

  int next = (actualPriority) % CANT_PRIORITIES;
  if (priorities[next].actual != NULL)
    priorities[next].actual = priorities[next].actual->next;
  while (!priorities[next].actual ||
         !priorities[next].actual->data.flagRunning) {
    if (!priorities[next].actual) {
      priorities[next].actual = priorities[next].first;
      next = (next + 1) % CANT_PRIORITIES;
    } else
      priorities[next].actual = priorities[next].actual->next;
  }
  return next;
}

long exitProces() {
  killProcess(priorities[actualPriority].actual->data.pid);
  return switchContext(0);
}
int getProcesses() { return processesRunning; }
int killProcess(int pid) {
  for (int i = 0; i < CANT_PRIORITIES; i++) {
    Node *aux = priorities[i].first;
    Node *prev = priorities[i].first;
    while (aux != NULL) {
      if (aux->data.pid == pid) {
        processesRunning -= 1;
        prev->next = aux->next;
        if (aux == priorities[i].first)
          priorities[i].first = aux->next;
        if (aux == priorities[i].actual)
          priorities[i].actual = aux->next;
        freeMemory(aux);
        return processesRunning;
      }
      prev = aux;
      aux = aux->next;
    }
  }
  return processesRunning;
}

int pauseProces(int pid) {
  if (!priorities[actualPriority].first->data.flagPaused) {
    priorities[actualPriority].first->data.flagPaused = 1;
    processesPaused += 1;
  }

  return processesRunning;
}
int reloadProcess(int pid) {
  if (priorities[actualPriority].first->data.flagPaused) {
    priorities[actualPriority].first->data.flagPaused = 0;
    processesPaused -= 1;
  }
  return processesRunning;
}

/*
    Lo que hace esta funcion es buscar memoria para el stack y cargarlo dicho
   stack
*/
int loadFirstContext(void *funcPointer, int window, int argC, char **argv) {

  int newProcessPriority = 0;
  // Lo hago de esta manera para que la shell tenga una prioridad mayor
  if (processesRunning)
    newProcessPriority = DEFAULT_PRIORITY;

  addNewProcess(newProcessPriority);
  // TODO esto se podria poner en ADT de la lista para que la misma maneje estas
  // cuestiones

  // Aca deberia hacer una alloc pero lo dejo para luego
  priorities[newProcessPriority].first->data.stackPointer =
      stacks[processesRunning];
  priorities[newProcessPriority].first->data.stackPointer += MAX_STACK - 1;
  priorities[newProcessPriority].first->data.flagRunning = 1;
  priorities[newProcessPriority].first->data.flagPaused = 0;
  priorities[newProcessPriority].first->data.pid = nextProcessPid++;
  priorities[newProcessPriority].first->data.quantum =
      prioritiesQuatums[newProcessPriority];

  priorities[newProcessPriority].first->data.stackPointer = loadContext(
      window, argC, argv,
      priorities[newProcessPriority].first->data.stackPointer, funcPointer);
  processesRunning += 1;
  return priorities[newProcessPriority].first->data.pid;
}

void addNewProcess(int newProcessPriority) {
  if (priorities[newProcessPriority].first == NULL) {
    priorities[newProcessPriority].first = allocMemory(sizeof(Node));
    priorities[newProcessPriority].actual =
        priorities[newProcessPriority].first;
  } else {
    Node *aux = priorities[newProcessPriority].first;
    priorities[newProcessPriority].first = allocMemory(sizeof(Node));
    priorities[newProcessPriority].first->next = aux;
  }
}

int getFD(int contextOwner) {
  return priorities[actualPriority].first->data.fileDescriptor;
}
