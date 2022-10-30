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

#define MAX_NAME 255
#include "list.h"
#include "stddef.h"
#include "pidQueue.h"
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
  char ** argV;
  pidQueue * waitingPidList;
} PCB;

/*
 *Defino un array de los diferentes niveles de procesos
 *Por default la jerarquia del proceso va a ser 2
 */
static struct head *psReady[CANT_PRIORITIES];

static struct head *psWaiting[CANT_PRIORITIES];

static PCB *currentProcess;
/*
 *Puntero a la lista en donde vamos a tener los procesos qe estan esperando
 *por su hijo
 */
static struct head *psBlocked = NULL;
/*
 *Defino un array statico el cual va a guardar los quatums que va a tener cada
 *nivel de privilegios
 */
static int prioritiesQuatums[] = {10, 7, 5, 3, 1};
/*
 *Variable que nos dice en que prioridad estamos corriendo
 *La seteo en 0 pues es la prioridad mas importante y es en la que vamos a
 *trabajar
 */
static int actualPriority = 0;
/*
 *Esta variable nos permite generar un pid para cada proceso
 */
static unsigned long nextProcessPid = 0;
/*
    TODO Deberia eliminar esto
*/
static int contextOwner = -1;
/*
 * Cuento la
 */
static int ticks = 0;

static int currentQuantum = 0;

static char stacks[10][MAX_STACK];

/*
 * Defino la cantidad de procesos que tengo corriendo en este mismo momento
 * Obs: siempre tiene que ser menor que MAX_PROCESES
 */
static int processesRunning = 0;
static int processesPaused = 0;

/*

*/
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
long exitProces();
/*
 * Funcion la cual va a recibir el contexto para iniciar un nuevo proceso
 * Devuelve el PID del Proceso en cuestion
 */
int loadFirstContext(void *funcPointer, int window, int argC, char **argv,
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
PCB *searchAndDelete(int pid);
void nice(int pid, int priority);
int getProcesses();
int unblockProcess(int pid);
void psDump();

void autoBlock(int pidToWait);

void freeProcess(PCB *toFree);
void sendToBlockedList();
void setActualPriority();
int currentPid();
void yield();
#endif
