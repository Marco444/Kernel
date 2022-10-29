#include "../include/list.h"
#include "../include/naiveConsole.h"
head *newList() {
  head *toReturn = allocMemory(sizeof(head));
  toReturn->first = NULL;
  return toReturn;
}

void push(head *list, dataType *data) {
  Node *newProcess = allocMemory(sizeof(Node));
  newProcess->data = data;
  newProcess->next = NULL;
  if (list->first == NULL) {
    list->first = newProcess;
    list->last = newProcess;
  } else {
    list->last->next = newProcess;
    list->last = newProcess;
  }
}

dataType *deleteNode(head *list, int pid) {

  if (list->first == NULL) {
    return;
  }
  Node *current = list->first;
  Node *prev = NULL;
  while (current != NULL) {
    if (current->data != NULL && current->data->pid == pid) {
      if (current == list->first)
        list->first = current->next;
      if (current == list->last)
        list->last = prev;
      if (prev != NULL)
        prev->next = current->next;

      dataType *toReturn = current->data;
      freeMemory(current);
      return toReturn;
    }
    prev = current;
    current = current->next;
  }
  return NULL;
}

dataType *pop(head *list) {
  if (list->first != NULL) {

    dataType *toReturn = list->first->data;
    Node *aux = list->first;
    if (list->first == list->last) {
      list->first = NULL;
      list->last = NULL;
    } else {
      list->first = aux->next;
    }
    free(aux);
    return toReturn;
  }

  return NULL;
}

dataType *next(head *list) {
  /* Node *toReturn = list->actual;
   list->actual = toReturn->next;
   return toReturn->data;
   */
}
dataType *peek(head *list) {
  if (list->first != NULL) {
    return list->first->data;
  }

  return NULL;
}

dataType *getNode(head *list, int pid) {
  Node *aux = list->first;
  while (aux) {
    if (aux->data) {
      if (aux->data->pid == pid)
        return aux->data;
    }
  }
  return NULL;
}
dataType *getNodeWaiting(head *list, int WaitingPid) {
  Node *aux = list->first;
  while (aux) {
    if (aux->data) {
      if (aux->data->waitingPid == WaitingPid)
        return aux->data;
    }
  }
  return NULL;
}

void dumpList(head *list) {
  Node *first = list->first;
  while (first) {
    if (first->data) {
      ncPrint(first->data->name);
      ncPrint("     ");
      ncPrintDec(first->data->pid);
      ncPrint("     ");
      ncPrintHex(first->data->stackBase);
      ncPrint("     ");
      ncPrintHex(first->data->stackPointer);
      ncPrint("    ");
      ncPrintDec(first->data->type);
      ncPrint("    ");
      ncPrintDec(first->data->priority);
      ncPrint("\n");
    }
    first = first->next;
  }
}
void pushAll(head *toList, head *fromList) {
  if (toList->last == NULL) {
    toList->first = fromList->first;
    toList->last = fromList->last;
  } else {
    toList->last->next = fromList->first;
    toList->last = fromList->last;
  }
}
void cleanAll(head *list) {
  list->first = NULL;
  list->last = NULL;
}