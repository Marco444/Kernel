// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
#include "../include/list.h"

#include "../include/naiveConsole.h"
head *newList() {
  head *toReturn = alloc(sizeof(head));
  toReturn->first = NULL;
  return toReturn;
}

void push(head *list, Node *data) {
  if (data == NULL) return;
  data->next = NULL;
  if (list->first == NULL) {
    list->first = data;
    list->last = data;
  } else {
    list->last->next = data;
    list->last = data;
  }
}

Node *deleteNode(head *list, int pid) {
  if (list->first == NULL) {
    return NULL;
  }
  Node *current = list->first;
  Node *prev = NULL;
  while (current != NULL) {
    if (current->data != NULL && current->data->pid == pid) {
      if (current == list->first) list->first = current->next;
      if (current == list->last) list->last = prev;
      if (prev != NULL) prev->next = current->next;

      return current;
    }
    prev = current;
    current = current->next;
  }
  return NULL;
}

Node *pop(head *list) {
  if (list->first != NULL) {
    Node *aux = list->first;
    if (list->first == list->last) {
      list->first = NULL;
      list->last = NULL;
    } else {
      list->first = aux->next;
    }
    return aux;
  }

  return NULL;
}

Node *peek(head *list) {
  if (list->first != NULL) {
    return list->first;
  }

  return NULL;
}

Node *getNode(head *list, int pid) {
  Node *aux = list->first;
  while (aux) {
    if (aux->data) {
      if (aux->data->pid == pid) return aux;
    }
  }
  return NULL;
}
Node *getNodeWaiting(head *list, int WaitingPid) {
  Node *aux = list->first;
  while (aux) {
    if (aux->data) {
      if (aux->data->waitingPid == WaitingPid) return aux;
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
