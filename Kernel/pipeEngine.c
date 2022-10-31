#include "include/pipeEngine.h"
#include "include/MemoryManager.h"
#include "include/naiveConsole.h"
#include "include/schedluerEngine.h"
#include "include/semaphores.h"

/* struct para mantener todos los pipes del sistema corriendo */
struct pipeEngine {
  struct pipe pipes[MAX_PIPE_NUMBER];
  int next;
};

// struct pipeEngine PipeEngine = {.next = 0};
//
// Pipe allocPipe() {
//   if(PipeEngine.next > MAX_PIPE_NUMBER) return NULL;
//   return &PipeEngine.pipes[PipeEngine.next++];
// }

/* mantengo una lista con todos los fd del sistema */
struct fdEngine {
  File fds[MAX_FD_COUNT];
  int next;
};

// struct fdEngine FdEngine = {.next = 0};
//
// File * allocFileDescriptor() {
//   if(FdEngine.next > MAX_FD_COUNT) return NULL;
//   return &FdEngine.fds[FdEngine.next++];
// }

void wakeup(Pipe p, char type) {

  int startIdx = p->next;

  while (p->next != startIdx && p->blocked[p->next].type != type)
    p->next = (p->next + 1) % MAX_BLOCKED;

  if (p->blocked[p->next].type == type) {
    unblockProcess(p->blocked[p->next].pid);
  }
}

void sleep(Pipe p, char type) {

  // me marco como durmiendo en la lista
  p->next = (p->next + 1) % MAX_BLOCKED;
  p->blocked[p->next].pid = currentPid();
  p->blocked[p->next].type = type;

  // libero el lock porque me voy a dormir
  semSignal(p->lock);

  // le digo al scheduler que me fui a dormir
  blockProcess(p->blocked[p->next].pid);
}

/* se crea un pipe a partir de dos punteros a fd,
 * alocando memoria para ellos tambien. */
int pipe(File *f0, File *f1) {

  // defino un nuevo Pipe para comunicar f0 y f1
  // Pipe p = allocPipe();
  //
  // // pido memoria para los dos file descriptors
  // // con los cuales voy a usar el pipe
  // f0 = allocFileDescriptor();
  // f1 = allocFileDescriptor();
  //
  // if(f0 == NULL || f1 == NULL || p == NULL) return -1;
  //
  // p->readopen = 1;
  // p->writeopen = 1;
  // p->nwrite = 0;
  // p->nread = 0;
  // p->next = 0;
  //
  // p->lock = semOpen(getNextAvailableSemaphore());
  //
  // (*f0)->type = FD_PIPE;
  // (*f0)->readable = 1;
  // (*f0)->writable = 0;
  // (*f0)->pipe = p;
  //
  // (*f1)->type = FD_PIPE;
  // (*f1)->readable = 0;
  // (*f1)->writable = 1;
  // (*f1)->pipe = p;
  //
  return 0;
}

void pipeclose(Pipe p, int writable) {
  semWait(p->lock);
  if (writable) {
    p->writeopen = 0;
    wakeup(p, READER);
  } else {
    p->readopen = 0;
    wakeup(p, WRITER);
  }
  if (p->readopen == 0 && p->writeopen == 0) {
    semSignal(p->lock);
  } else
    semSignal(p->lock);
}

int pipewrite(Pipe p, char *addr, int n) {
  int i;

  // conseguimos el lock del pipe
  semWait(p->lock);

  for (i = 0; i < n; i++) {

    // mientras el pipe este lleno
    while (p->nwrite == p->nread + PIPESIZE) { // DOC: pipewrite-full

      // aca libero el pipe si no hay procesos leyendo del pipe! me
      // retorna EOF a lo linux.
      //  if(p->readopen == 0 || myproc()->killed){
      //    release(&p->lock);
      //    return -1;
      //  }

      // levanto a cualquier proceso que tenga que leerlo
      wakeup(p, READER);

      // nos vamos a dormir porque no puedo escribir
      // en la condicion p->write y el sleep libera
      // el lock (por eso se lo paso tambien como
      // parametro)
      sleep(p, WRITER); // DOC: pipewrite-sleep
    }

    // eventualmente voy a poder escribir, por lo tanto
    // escribo un byte y vuelvo a chequer
    p->data[p->nwrite++ % PIPESIZE] = addr[i];
  }

  // luego de haber escrito todo despierto a cualquiera que necesite leer
  wakeup(p, READER); // DOC: pipewrite-wakeup1

  // y dejo el lock
  semSignal(p->lock);
  return n;
}

int piperead(Pipe p, char *addr, int n) {
  int i;

  semWait(p->lock);

  // si esta vacio el pipe
  while (p->nread == p->nwrite && p->writeopen) { // DOC: pipe-empty

    // si no tengo proceso escribiendo termino porque
    // nada que leer -> nota de eficiencia
    //  if(myproc()->killed){
    //    release(&p->lock);
    //    return -1;
    //  }

    // me voy a dormir y con el lock
    // lo libero desde sleep (cuando
    /// me levanten me liberan el lock)
    sleep(p, READER); // aca no la puedo cagar si switcheo??
  }

  // voy por todos los bytes que tengo que leer
  for (i = 0; i < n; i++) { // DOC: piperead-cop

    // si leo todos termino
    if (p->nread == p->nwrite)
      break;

    // sino voy leo
    addr[i] = p->data[p->nread++ % PIPESIZE];
  }

  // por ultimo levanto al proceso escritor
  //(por que es necesario? porque puede haberse
  // bloqueado por no tener espacio para escribir)
  wakeup(p, WRITER); // DOC: piperead-wakeup

  // libero el lock
  semSignal(p->lock);

  // devuelvo cantidad de bytes leidos
  return i;
}

void pipesDump() { ncPrint("pipes dumped! \n"); }
