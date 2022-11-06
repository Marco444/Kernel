// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
#include "include/pipeEngine.h"

#include "include/MemoryManager.h"
#include "include/fileDescriptorManager.h"
#include "include/naiveConsole.h"
#include "include/schedluerEngine.h"
#include "include/semaphores.h"

char eof = EOF;

/* struct para mantener todos los pipes del sistema corriendo
 * se mantiene como arreglo circular para asi poder reutilizar
 * los pipes a medida que se van cerrando */
struct pipeEngine {
  struct pipe pipes[MAX_PIPE_NUMBER];
  int next;
};

struct pipeEngine PipeEngine;

void initPipeEngine() {
  for (int i = 0; i < MAX_PIPE_NUMBER; i++) PipeEngine.pipes[i].closed = 0;
  PipeEngine.next = 0;
}

void initPipe(Pipe p) {
  p->closed = 0;
  p->readopen = 1;
  p->writeopen = 1;
  p->next = 1;
  p->nwrite = 0;
  p->nread = 0;
  p->lock = semOpen(getNextAvailableSemaphore(), 1);
  // for(int i = 0; i < PIPESIZE; i++) p->data[i] = 0;
}

Pipe allocPipe() {
  int startIdx = PipeEngine.next;

  do {
    PipeEngine.next = (PipeEngine.next + 1) % MAX_PIPE_NUMBER;
  } while (startIdx != PipeEngine.next &&
           PipeEngine.pipes[PipeEngine.next].closed);

  if (startIdx == PipeEngine.next) return NULL;

  initPipe(&PipeEngine.pipes[PipeEngine.next]);

  return &PipeEngine.pipes[PipeEngine.next];
}

void wakeup(Pipe p, char type) {
  int startIdx = p->next;

  do {
    p->next = (p->next + 1) % MAX_BLOCKED;
  } while (p->next != startIdx && p->blocked[p->next].type != type);

  if (p->blocked[p->next].type == type) unblockProcess(p->blocked[p->next].pid);
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

int pipe(int fd[2]) {
  // defino un nuevo Pipe para comunicar f0 y f1
  Pipe p = allocPipe();

  // pido memoria para los dos file descriptors
  // con los cuales voy a usar el pipe
  File f0 = allocFileDescriptor();
  File f1 = allocFileDescriptor();

  if (f0 == NULL || f1 == NULL || p == NULL) return -1;

  f0->type = FD_PIPE;
  f0->readable = 0;
  f0->writable = 1;
  f0->pipe = p;
  fd[0] = f0->id;

  f1->type = FD_PIPE;
  f1->readable = 1;
  f1->writable = 0;
  f1->pipe = p;
  fd[1] = f1->id;

  return 0;
}

void pipeclose(Pipe p, int writable) {
  semWait(p->lock);

  if (writable) {
    p->writeopen--;
    wakeup(p, READER);
  } else {
    p->readopen = 0;
    wakeup(p, WRITER);
  }

  if (p->writeopen == 0) {
    semSignal(p->lock);
    pipewrite(p, &eof, 1);
    p->closed = 1;
  } else
    semSignal(p->lock);
}

int pipewrite(Pipe p, char *addr, int n) {
  int i;

  // conseguimos el lock del pipe
  semWait(p->lock);

  for (i = 0; i < n; i++) {
    // mientras el pipe este lleno
    while (p->nwrite == p->nread + PIPESIZE) {
      // levanto a cualquier proceso que tenga que leerlo
      wakeup(p, READER);

      // nos vamos a dormir porque no puedo escribir
      // en la condicion p->write y el sleep libera
      // el lock (por eso se lo paso tambien como
      // parametro)
      sleep(p, WRITER);
    }

    // eventualmente voy a poder escribir, por lo tanto
    // escribo un byte y vuelvo a chequer
    p->data[p->nwrite++ % PIPESIZE] = addr[i];
  }

  // luego de haber escrito todo despierto a cualquiera que necesite leer
  wakeup(p, READER);

  // y dejo el lock
  semSignal(p->lock);
  return n;
}

int piperead(Pipe p, char *addr, int n) {
  int i;

  semWait(p->lock);

  // si esta vacio el pipe
  while (p->nread == p->nwrite && p->writeopen) {  // DOC: pipe-empty

    // me voy a dormir y con el lock
    // lo libero desde sleep (cuando
    /// me levanten me liberan el lock)
    sleep(p, READER);  // aca no la puedo cagar si switcheo??
  }

  // voy por todos los bytes que tengo que leer
  for (i = 0; i < n; i++) {
    // si leo todos termino
    if (p->nread == p->nwrite) break;

    // sino voy leo
    addr[i] = p->data[p->nread++ % PIPESIZE];
  }

  // por ultimo levanto al proceso escritor
  //(por que es necesario? porque puede haberse
  // bloqueado por no tener espacio para escribir)
  wakeup(p, WRITER);  // DOC: piperead-wakeup

  // libero el lock
  semSignal(p->lock);

  // devuelvo cantidad de bytes leidos
  return i;
}

void printProcess(Process p) {
  ncPrint("    pid: ");
  ncPrintDec(p.pid);
  ncPrint(", type: ");
  ncPrint(p.type == READER ? "reader" : "writer");
  ncNewline();
}

void printBlockedProcesses(struct pipe p) {
  ncPrint("Bloqueados: ");
  for (int i = 0; p.blocked[i].pid != 0; i++) {
    printProcess(p.blocked[i]);
  }
}

void pipesDump() {
  for (int i = 0; i < PipeEngine.pipes[i].next; i++) {
    ncPrint("pipe #");
    ncPrintDec(i);
    ncNewline();
    printBlockedProcesses(PipeEngine.pipes[i]);

    ncNewline();
  }
}
