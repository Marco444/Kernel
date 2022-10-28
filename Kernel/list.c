#include "../include/list.h"
#include "../include/naiveConsole.h"
head *newList() {
  head *toReturn = allocMemory(sizeof(head));
  toReturn->actual = NULL;
  toReturn->first = NULL;
  return toReturn;
}

void push(head *list, dataType *data) {
  Node *aux = list->first;
  list->first = allocMemory(sizeof(Node));
  list->first->data = data;
  list->first->next = aux;
  list->first->before = NULL;
  if (aux != NULL)
    aux->before = list->first;
  if (list->actual == NULL)
    list->actual = list->first;
}

dataType *deleteNode(head *list, int pid) {

  Node *aux = list->first;

  while (aux != NULL) {
    if (aux->data->pid == pid) {
      if (aux->before != NULL && aux->next != NULL) {
        aux->before->next = aux->next;
        aux->next->before = aux->before;
      } else if (aux->before == NULL && aux->next == NULL) {
        list->first = NULL;
      } else if (aux->before == NULL) {
        list->first = aux->next;
        list->first->before = NULL;
      } else {
        aux->before->next = NULL;
      }
      if (aux == list->actual)
        list->actual = aux->next;
      dataType *toReturn = aux->data;
      freeMemory(aux);
      return toReturn;
    }
    aux = aux->next;
  }
  return NULL;
}

dataType *deleteCurrentProcess(head *list) {
  if (list->actual == NULL) {
    return NULL;
  }

  Node *aux = list->actual;
  if (aux->before && aux->next) {
    aux->before->next = aux->next;
    aux->next->before = aux->before;
  } else if (!aux->before && !aux->next) {
    list->first = NULL;
  } else if (!aux->before) {
    list->first = aux->next;
    aux->next->before = NULL;
  } else {
    aux->before->next = NULL;
  }
  list->actual = aux->next;
  dataType *toReturn = aux->data;
  freeMemory(aux);
  return toReturn;
}

dataType *next(head *list) {
  Node *toReturn = list->actual;
  list->actual = toReturn->next;
  return toReturn->data;
}
dataType *current(head *list) { return list->actual->data; }
int hasNext(head *list) {
  return list->actual != NULL && list->actual->data != NULL;
}

void initializeIterator(head *list) { list->actual = list->first; }

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