#ifndef SCHEDLUER_ENGINE_H
#define SCHEDLUER_ENGINE_H

#define MAX_PROCESSES 3

#define MAX_STACK 1200
#define REGISTER_COUNT 18

#define RSP 7               // defino el indice donde guardo el RSP en mi arreglo de registros
#define RDI 5               // defino el indice en donde guardo el registro RDI
#define SHELL 0

#define TICKS 1

/*
 * Definido el formato donde voy a almacenar los contextos de los diferentes proyectos que van a correr,
 * una array que en cada indice va a guardar el valor de cada uno de los registro. 
 */
typedef struct {
    long registers[REGISTER_COUNT];
} Context;

/*
 * Defino el formato que voy a utilizar para almacenar los procesos en mi tabla para hacer context switching
 */
typedef struct {
    Context context;
    int flagRunning;
    int flagPaused;
    int fileDescriptor; // TODO DEBEMOS HACER UNA TABLA PARA LOS FD QUE DEBE utiLIZAR 
    char  * stackFrame;
} Process;
          

/* 
* Defino el array para guardar los contextos de todos los procesos que puedo switchear al mismo tiempo.
 * Obs: si tendriamos memoria dinamica esto quedaria mejor.
*/
Process * procesos;

/*
 * Cuento la 
 */
static int ticks = 0;

static char stacks[10][1200];

/*
    * Defino la cantidad de procesos que tengo corriendo en este mismo momento
    * Obs: siempre tiene que ser menor que MAX_PROCESES
*/
int processesRunning = 0;
int processesPaused = 0;

/*
 * Devuelve si hay que hacer un switch de contexto o no
 */
int toSwitch();

/**/
void initialiseContextSchedluerEngine();

/*
 * Funcion que recibe el puntero a posicion de memoria donde tengo el ultimo contexto y el
 * proceso que es duenio de ese contexto. Luego sobreescribe en el contextHolder el contexto
 * del proximo proceso, y en el contextOwner copia el identificador del contexto.
 */
void switchContext(long * contextHolder, int * contextOwner);

/*
    * Funcion la cual va a copiar todos los registros mandados desde assembler
    * a la struct en el array
*/
static void pushContext(long * contextHolder, int contextOwner);
static void popContext(long * contextHolder, int contextOwner);
/*
    * Funcion que recibe el numero de duenio del contexto para sacarlo del array
*/
int exitProces(long * contextHolder,int * contextOwner);
/*
    * Funcion la cual va a recibir el contexto para iniciar un nuevo proceso
    * obs: si ya hay mas de 2 procesos no se lo agrega
*/
void loadFirstContext(long * contextHolder);
char  nextProcess(int * contextOwner ) ;
/*
    * Funcion la cual pausa un proceso

    *Args: int PID
*/
int pauseProces(int pid);
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
#endif