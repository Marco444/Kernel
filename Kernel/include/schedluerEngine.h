#ifndef SCHEDLUER_ENGINE_H
#define SCHEDLUER_ENGINE_H

#define CANT_PRIORITIES 5

#define MAX_STACK 4096

#define SHELL 0

#define TICKS 1

#define DEFAULT_PRIORITY 2

#define BLOCK 1

#define KILL 2

#define READY 0

#define BACKGROUND 1

#define FOREGROUND 0

#define MAX_ARGUMENT_LENGTH 30

#define MAX_NAME 255
#include "fileDescriptorManager.h"
#include "lib.h"
#include "list.h"
#include "pidQueue.h"
#include "stddef.h"

/*
 * Defino el formato que voy a utilizar para almacenar los procesos en mi tabla
 * para hacer context switching
 */
typedef struct pcb {
  int quantum; // Este campo es para saber cuanto le queda para que termine de
               // correr
  int priority;
  int fileDescriptor; // TODO DEBEMOS HACER UNA TABLA PARA LOS FD QUE DEBE
  long stackPointer;
  unsigned long pid;
  int state;
  int type;
  int waitingPid;
  long stackBase;
  char *name;
  int argC;
  char **argV;
  File fd[2];
  pidQueue waitingPidList;
} PCB;

/*

*/
void *checkAlloc(int size);
void initialiseContextSchedluerEngine();

/*
 * Funcion que recibe el puntero a posicion de memoria donde tengo el ultimo
 * contexto y el proceso que es duenio de ese contexto. Luego sobreescribe en el
 * contextHolder el contexto del proximo proceso, y en el contextOwner copia el
 * identificador del contexto.
 */
long switchContext(long rsp);

/*
 * Funcion que recibe el numero de duenio del contexto para sacarlo del array
 */
void exitProces();
/*
 * Funcion la cual va a recibir el contexto para iniciar un nuevo proceso
 * Devuelve el PID del Proceso en cuestion
 */
int loadFirstContext(void *funcPointer, int window, int argC, char argv[20][20],
                     int type, char *name);
/*
 *Funcion la cual va a agregar un nuevo proceso a la lista de prioridades
 *Parama: int en que prioridad se lo quiere agregar.
 */
void addNewProcess(int newProcessPriority);
char nextProcess();

/*
 *Funcion la cual elimina un proceso
 *Args: pint PID
 */
int killProcess(int pid);
/*
    *Funcion la cual hace que vuelva a correr un proceso

    *Args : int PID
*/
int reloadProcess(int pid);
/*

    *Funcion la cual devuelve el FD de un proceso segun su PID

    * Args = int pid
    *Return = int FD

*/
int getFD(int contexOwner);
/*
    Funcion la cual recibe la cantidad de procesos corriendo

    *return = int cant

*/
int blockProcess(int pid);
struct Node *searchAndDelete(int pid);
void nice(int pid, int priority);
int getProcesses();
int unblockProcess(int pid);
void psDump();

void bockCurrentProcess(int pidToWait);

void freeProcess(struct Node *toFree);
void sendToBlockedList();
void setActualPriority();
int currentPid();
void yield();
void meRompi();
char **loadArgs(int argC, char argV[20][20]);
#endif
