#define PIPESIZE 512

#define FD_PIPE 11
#define MAX_BLOCKED 20

#define READER 1
#define WRITER 0

typedef struct spinlock {

} * Spinlock;

char getPid();

void sleepProcess(char pid);

void wakeupProcess(char pid);

void initlock(struct spinlock *lock, char *name);

void acquire(Spinlock lock);

void release(Spinlock lock);

typedef struct process {
  char type, pid;
} Process;

typedef struct pipe {
  Spinlock lock;
  char data[PIPESIZE];
  Process blocked[MAX_BLOCKED]; // probably better to be a linked list
  int next;
  unsigned int nread, nwrite;
  int readopen, writeopen;
} * Pipe;

struct file {
  char type, readable, writable;
  Pipe pipe;
};

void wakeup(Pipe p, char type) {

  int startIdx = p->next;

  while (p->next != startIdx && p->blocked[p->next].type != type)
    p->next = (p->next + 1) % MAX_BLOCKED;

  if (p->blocked[p->next].type == type) {
    wakeupProcess(p->blocked[p->next].pid);
  }
}

void sleep(Pipe p, char type) {

  // me marco como durmiendo en la lista
  p->next = (p->next + 1) % MAX_BLOCKED;
  p->blocked[p->next].pid = getPid();
  p->blocked[p->next].type = type;

  // libero el lock porque me voy a dormir
  release(p->lock);

  // le digo al scheduler que me fui a dormir
  sleepProcess(p->blocked[p->next].pid);
}

Pipe pipe() {

  Pipe p = allocMemory(sizeof(struct Pipe));

  p->next = 0;

  initlock(p->lock, "pipe");

  (*f0)->type = FD_PIPE;
  (*f0)->readable = 1;
  (*f0)->writable = 0;
  (*f0)->pipe = p;

  (*f1)->type = FD_PIPE;
  (*f1)->readable = 0;
  (*f1)->writable = 1;
  (*f1)->pipe = p;

  return 0;
}

void pipeAs(Pipe p, int type) {
  acquire(p->lock);
  sleep(p, type);
  // if(writable){
  //   p->writeopen = 0;
  //   wakeup(p, READER);
  // } else {
  //   p->readopen = 0;
  //   wakeup(p, WRITER);
  // }
  // if(p->readopen == 0 && p->writeopen == 0){
  //   release(p->lock);
  // } else
  //   release(p->lock);
}

int pipewrite(Pipe *p, char *addr, int n) {
  int i;

  // conseguimos el lock del pipe
  acquire(p->lock);

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
  release(p->lock);
  return n;
}

int piperead(Pipe p, char *addr, int n) {
  int i;

  acquire(p->lock);

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
  release(p->lock);

  // devuelvo cantidad de bytes leidos
  return i;
}
