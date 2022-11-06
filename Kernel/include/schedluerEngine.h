#ifndef SCHEDLUER_ENGINE_H
#define SCHEDLUER_ENGINE_H

#define CANT_PRIORITIES 5

#define MAX_STACK 4096

#define MAX_FD_PROCESS 5

#define SHELL 0

#define TICKS 1

#define DEFAULT_PRIORITY 2

#define BLOCK 1

#define KILL 2

#define READY 0

#define BACKGROUND 1

#define FOREGROUND 0

#define MAX_ARGUMENT_LENGTH 20

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
  int quantum;  // Este campo es para saber cuanto le queda para que termine de
                // correr
  int priority;
  int fileDescriptor;  // TODO DEBEMOS HACER UNA TABLA PARA LOS FD QUE DEBE
  long stackPointer;
  unsigned long pid;
  int state;
  int type;
  int waitingPid;
  long stackBase;
  char *name;
  int argC;
  long stack;
  char argV[MAX_ARGUMENT_LENGTH][MAX_ARGUMENT_LENGTH];
  int fd[MAX_FD_PROCESS];
  pidQueue waitingPidList;
} PCB;

/*
 * Funcion la cual se encarga de inicializar el scheduler
 */
void initialiseContextSchedluerEngine();
/*
 * Declaracion del proceso idle, proceso el cual se ejecutara cuando no haya
 * otro proceso que ejecutar
 */
void idle(int arc, char argv[MAX_ARGUMENT_LENGTH][MAX_ARGUMENT_LENGTH]);
/*
 * Funcion la cual recibe los parametros para la creacion de la pcb
 */
PCB *createProcessPCB(int pid, int newProcessPriority, int type, char *name,
                      int argC,
                      char argv[MAX_ARGUMENT_LENGTH][MAX_ARGUMENT_LENGTH]);

/*
 * Funcion que recibe el puntero a posicion de memoria donde tengo el ultimo
 * contexto y el proceso que es duenio de ese contexto. Luego sobreescribe en el
 * contextHolder el contexto del proximo proceso, y en el contextOwner copia el
 * identificador del contexto.
 */
long switchContext(long rsp);

/*
 * Funcion  la cual saca al proceso que lo llamo del RR
 */
void exitProces();
/*
 * Funcion la cual va a recibir el contexto para iniciar un nuevo proceso
 * Devuelve el PID del Proceso en cuestion
 */
int loadFirstContext(void *funcPointer, int argC, char argv[20][20], int type,
                     char *name);

/*
 * Funcion la cual nos asigna cual es el proximo proceso a ejecutar
 */
void nextProcess();
/*
 *Funcion la cual elimina un proceso
 *Args: pint PID
 */
int killProcess(int pid);

/*
 * Funcion la cual se encarga de bloqeuar al proceso con pid
 */
int blockProcess(int pid);
/*
 * Funcion la cual busca un nodo en todas las listas y lo saca de la misma
 */
struct Node *searchAndDelete(int pid);
/*
 * Funcion la cual permite cambiar la prioridad de un proceso
 */
void nice(int pid, int priority);
/*
 * Funcion la cual devuelve la cantidad de procesos corriendo
 */
int getProcesses();
/*
 * Funcion la cual desbloqeua al proceso con el pid en cuestion
 */
int unblockProcess(int pid);
/*
 * Funcion la cual imprime todos los procesos en el RR en ese instante
 */
void psDump();
/*
 * Funcion la cual bloquea el proceso con el pid
 */
void bockCurrentProcess(int pidToWait);
/*
 * Funcion la cual ese el free completo de un proceso
 */
void freeProcess(struct Node *toFree);
/*
 * Funcion la cual lo manda a la lista de bloqueados
 */
void sendToBlockedList();

void setActualPriority();
/*
 * Funcion la cual devuelve el pid del proceso que se esta corriendo en ese
 * instante
 */
int currentPid();
/*
 * Funcion la cual el proceso actual deja el procesador
 */
void yield();
/*
 * Funcion la cual desbloquea a todos lo procesos que esperan a un proceso por
 * el waitPid
 */
void unblockChilds();
/*
 * Funcion la cual deveulve la tabla de Fds del proceso actual
 */
int *currentProcessFds();

#endif
