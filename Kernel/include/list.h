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

void push(head *list, Node *data);

Node *deleteNode(head *list, int pid);

Node *deleteCurrentProcess(head *list);

dataType *next(head *list);

int hasNext(head *list);

void initializeIterator(head *list);

Node *getNode(head *list, int pid);

Node *getNodeWaiting(head *list, int WaitingPid);

void dumpList(head *list);

Node *pop(head *list);
Node *peek(head *list);
void pushAll(head *toList, head *fromList);
void cleanAll(head *list);
#endif