#include "../include/phylo.h"

#include <stddef.h>

#include "../include/_string.h"
#include "../include/commandsEngine.h"
#include "../include/stdio.h"
#include "../include/syscalls.h"

typedef struct PhyloCDT{
    int status;
    int pos;
    int pid;
}PhyloCDT;

typedef struct PhyloCDT* Phylo;

static Phylo phylos[MAX_PHYLOS];
static Semaphore forks[MAX_FORKS];
static Semaphore phylosActive[MAX_PHYLOS];

static int phylosCount = 0;

static void listenInput();
static Phylo newPhilosopher(int pos);
static void newFork(int pos);
static void deleteAllPhilosophers();
static void closeAllForks();
static void printTable();
static void blockPhylo();
static void unblockPhylo();

void initializePhylos(int argc, char argv[MAX_ARGUMENT_COUNT][MAX_ARGUMENT]){

    puts_("Iniciando Phylos ... \n");
    puts_("Presione:\n   -'a' para agregar un filosofo \n   -'r' para remover un filosfo \n   -'q' para finalizar \n ");
    puts_("El numero de filosofos debe estar entre 5 y 10\n");
    printHeader("      ' E ': Comiendo - ' X ': Esperando para comer - ' . ': Pensando", WHITE | GREEN_BACKGROUND);

    // Highest priority to phylos manager
    sysNiceProcess(sysGetCurrentPid(), 1);
    
    // Create the first 5 philosophers
    for(int i = 0; i < MAX_PHYLOS; i++){
        phylos[i] = newPhilosopher(i);
        // Lowest priority for each phylo
        sysNiceProcess(phylos[i]->pid, 4);
        if(i < MIN_PHYLOS){
            phylosActive[i] = semOpen(i + MAX_PHYLOS, 1);
            phylos[i]->status = THINKING;
        }
        else{
            phylosActive[i] = semOpen(i + MAX_PHYLOS, 0);
            phylos[i]->status = SLEEPING;
        }
    }
    
    phylosCount = 5;

    printTable();

    listenInput();
    
}

static void fakeSleep(){
    for(int i = 0; i < 90000000; i++);
}


void philosopher(int argc, char argv[20][20]){

    if(argc != 2)
        return;

    int id = atoi_(argv[1]);
    // The philosopher will eat, think and repeat
    while(1){
        semWait(phylosActive[id]);
        phylos[id]->status = THINKING;
        // Think
        fakeSleep(); 
        // Think
        
        int left = (id - 1) % phylosCount;
        int right = id;


        phylos[id]->status = HUNGRY;
        
        // Try to eat
        if(id % 2 == 0){
            semWait(forks[left]);
            semWait(forks[right]);
        }else{
            semWait(forks[right]);
            semWait(forks[left]);
        }

        phylos[id]->status = EATING;
        
        printTable();
        
        // Eating
        fakeSleep();
        // Eating

        // Finished eating 
        if(id % 2 == 0){
            semSignal(forks[right]);
            semSignal(forks[left]);
        }else{
            semSignal(forks[left]);
            semSignal(forks[right]);
        }
        
        phylos[id]->status = THINKING;
        
        printTable();
        semSignal(phylosActive[id]);
    }
}

static void blockPhylo(){
    semWait(phylosActive[phylosCount]);
    phylos[phylosCount]->status = SLEEPING;
}

static void unblockPhylo(){
    phylos[phylosCount]->status = THINKING;
    semSignal(phylosActive[phylosCount]);
}

static void listenInput(){
    
    char key;
    while(1){
        read(&key, STDIN);
        switch (key)
        {
        case ADD:
            if(phylosCount == MAX_PHYLOS){
                puts_("No hay mas filosofos disponibles. Solo hasta 10 filosofos\n");
            }else{
                unblockPhylo();
                phylosCount++;
                printTable();
            }
            break;
        
        case REMOVE:
            if(phylosCount == MIN_PHYLOS){
                puts_("Minimo de 5 filosofos\n");
            }
            else{
                phylosCount--;
                blockPhylo();
                printTable();
            }
            break;

        case QUIT:
            deleteAllPhilosophers();
            closeAllForks();
            for(int i = 0; i < MAX_PHYLOS; i++){
                semClose(phylosActive[i]);
            }
            putDefaultHeader();
            exit_();
            break;

        default:
            break;
        }
    }

}

static Phylo newPhilosopher(int id){

    char argv[2][20];
    argv[0][0] = '\0';
    argv[1][0] = id + '0';
    argv[1][1] = 0;

    int newpid = loadProcess(philosopher, 2, argv, 1, "Filosofo");

    Phylo newPhylo = sysAlloc(sizeof(PhyloCDT));

    newPhylo->pid = newpid;
    newPhylo->status = THINKING;

    newFork(id);

    return newPhylo;
}

static void newFork(int pos){
    if((forks[pos] = semOpen(pos + 1, 1)) == NULL){
        puts_("Hubo un error al cerrar el semaforo (semOpen)\n");
        deleteAllPhilosophers();
        closeAllForks();
        exit_();
    }
}

static void deleteAllPhilosophers(){
    for(int i = 0; i < MAX_PHYLOS; i++){
      sysKillProcess(phylos[i]->pid);
      sysFree(phylos[i]);        
    }
}

static void closeAllForks(){
    for(int i = 0; i < MAX_FORKS; i++){
        if(forks[i] != NULL)
            semClose(forks[i]);
    }
}

static void printTable(){
    puts_("\n    ");
    int i = 0;
    while(i < MAX_PHYLOS && phylos[i] != NULL){

        switch (phylos[i]->status)
        {
        case EATING:
            puts_(" E ");
            break;
        
        case HUNGRY:
        puts_(" X ");
        break;

        case THINKING:
        puts_(" . ");
        break;

        case SLEEPING:
        puts_("   ");
        break;

        default:
            break;
        }
        i++;
    }
    puts_("\n");
}