#ifndef __LIST_H__
#define __LIST_H__

#include "MemoryManager.h"
#include "schedluerEngine.h"
typedef struct pcb dataType;

typedef struct Node {
  dataType *data;
  struct Node *next;
} Node;
typedef struct head {
  Node *first;
  Node *last;
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

dataType *pop(head *list);
dataType *peek(head *list);
void pushAll(head *toList, head *fromList);
void cleanAll(head *list);
#endif