#include "../include/queue.h"

head *newList() {
  head *toReturn = allocMemory(sizeof(head));
  toReturn->actual = NULL;
  toReturn->first = NULL;
  return toReturn;
}

void push(head *list, Process *data) {
  Node *aux = list->first;
  list->first = allocMemory(sizeof(Node));
  list->first->data = data;
  list->first->next = aux;
  list->first->before = NULL;
  if (aux)
    aux->before = list->first;
  if (!list->actual)
    list->actual = list->first;
}

Node *deleteNode(head *list, int pid) {

  Node *aux = list->first;

  while (aux) {
    if (aux->data->pid == pid) {
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
      if (aux == list->actual)
        list->actual = aux->next;
      return aux;
    }
  }
  return NULL;
}

Node *deleteCurrent(head *list) {
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
  return aux;
}

Node *next(head *list) {
  Node *toReturn = list->actual;
  list->actual = list->actual->next;
  return toReturn;
}
Node *actual(head *list) { return list->actual; }
int hasNext(head *list) { return list->actual != NULL; }

void initializeIterator(head *list) { list->actual = list->first; }

Node *getNode(head *list, int pid) {
  Node *aux = list->first;
  while (aux) {
    if (aux->data) {
      if (aux->data->pid == pid)
        return aux;
    }
  }
  return NULL;
}
Node *getNodeWaiting(head *list, int WaitingPid) {
  Node *aux = list->first;
  while (aux) {
    if (aux->data) {
      if (aux->data->waitingPid == WaitingPid)
        return aux;
    }
  }
  return NULL;
}