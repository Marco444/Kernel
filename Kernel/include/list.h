#ifndef __LIST_H__
#define __LIST_H__

#include "MemoryManager.h"
#include "schedluerEngine.h"
typedef struct pcb dataType;

typedef struct Node {
  dataType *data;
  struct Node *next;
  struct Node *before;
} Node;
typedef struct head {
  Node *first;
  Node *actual;
} head;

head *newList();

void push(head *list, dataType *data);

dataType *deleteNode(head *list, int pid);

dataType *deleteCurrentProcess(head *list);

dataType *next(head *list);

int hasNext(head *list);

void initializeIterator(head *list);

dataType *current(head *list);

dataType *getNode(head *list, int pid);

dataType *getNodeWaiting(head *list, int WaitingPid);

void dumpList(head *list);
#endif