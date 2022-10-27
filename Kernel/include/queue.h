#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "schedluerEngine.h"
//#include "MemoryManager.h"
typedef struct Node {
  struct Process *data;
  struct Node *next;
  struct Node *before;
} Node;
typedef struct head {
  Node *first;
  Node *actual;
} head;

head *newList();

void push(head *list, struct Process *data);

Node *deleteNode(head *list, int pid);

Node *deleteCurrent(head *list);

Node *next(head *list);

int hasNext(head *list);

void initializeIterator(head *list);

Node *actual(head *list);

Node *getNode(head *list, int pid);

Node *getNodeWaiting(head *list, int WaitingPid);

void dumpList(head *list);
#endif