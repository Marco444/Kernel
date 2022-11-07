// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.

#ifndef HEAP

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "../include/MemoryManager.h"
#include "../include/naiveConsole.h"

typedef struct list_t {
  uint8_t level;
  struct list_t *prev, *next;
} list_t;

#define MIN_ALLOC 5                  // 32 B min alloc
#define MAX_LEVELS (24 - MIN_ALLOC)  // 2^32 = 4GB.
#define BLOCKS_PER_LEVEL(level) (1 << (level))
#define SIZE_OF_BLOCKS_AT_LEVEL(level, total_size) \
  ((total_size) / (1 << (level)))
#define INDEX_OF_POINTER_IN_LEVEL(pointer, level, memory_start, total_size) \
  (((pointer) - (memory_start)) / (SIZE_OF_BLOCKS_AT_LEVEL(level, total_size)))
#define BIN_POW(x) (1 << (x))

static int log_2(size_t n);
static void list_init(list_t *list);
static int list_is_empty(list_t *list);
static void list_remove(list_t *entry);
static void list_push(list_t *list, list_t *entry);
static list_t *list_pop(list_t *list);
static list_t *belongs_to_list(list_t *buddy, uint8_t level);
static uint64_t list_free_space(uint8_t level);

static void addToLevel(list_t *list, list_t *node, uint8_t level);

static uint8_t getLevel(size_t bytes);
static int getFirstAvailableLevel(int minLevel);

static uint32_t maxMemSize = TOTAL_MEMORY;
static list_t allLists[MAX_LEVELS];
static uint8_t levels;
#define HEAP_SIZE TOTAL_HEAP_SIZE
static uint8_t *mem_base = (uint8_t *)(TOTAL_MEMORY - TOTAL_HEAP_SIZE);

void createMemoryManager() {
  levels = (int)log_2(maxMemSize) - MIN_ALLOC + 1;

  for (size_t i = 0; i < levels; i++) {
    list_init(&allLists[i]);
    allLists[i].level = i;
  }
  list_init((list_t *)mem_base);
  addToLevel(&allLists[0], (list_t *)mem_base, 0);
}

void *allocMemory(size_t size) {
  int minLevel = getLevel(size + sizeof(list_t));

  if (minLevel == -1) return NULL;

  void *ans, *node;
  list_t *link;

  int availableLevel = getFirstAvailableLevel(minLevel);

  if (availableLevel == -1) return NULL;

  while (availableLevel < minLevel) {
    void *left, *right;
    node = (void *)list_pop(&allLists[availableLevel]);
    uint64_t blockSize = SIZE_OF_BLOCKS_AT_LEVEL(availableLevel + 1, HEAP_SIZE);
    left = node;
    right = node + blockSize;
    list_init(left);
    list_init(right);
    list_push(&allLists[availableLevel + 1], left);
    list_push(&allLists[availableLevel + 1], right);
    availableLevel++;
  }

  link = list_pop(&allLists[minLevel]);
  link->level = minLevel;
  ans = link;
  ans += sizeof(list_t);
  return (void *)ans;
}

void freeMemory(void *address) {
  if (address == NULL) return;

  address -= sizeof(list_t);
  list_t *link = (list_t *)address, *buddy_link;
  uint8_t level = link->level;
  char stop = 0;

  do {
    link = (list_t *)address;
    size_t size = SIZE_OF_BLOCKS_AT_LEVEL(level, HEAP_SIZE);
    uint8_t index = INDEX_OF_POINTER_IN_LEVEL((uint64_t)address, level,
                                              (uint64_t)mem_base, HEAP_SIZE);
    uint64_t buddy;

    if ((index & 1) == 0) {
      buddy = (uint64_t)(address + size);
    } else {
      buddy = (uint64_t)(address - size);
    }

    buddy_link = NULL;
    if (!list_is_empty(&allLists[level]))
      buddy_link = belongs_to_list((list_t *)buddy, level);

    list_init(link);
    list_push(&allLists[level], link);

    if ((uint64_t)buddy_link == buddy) {
      list_remove(link);
      list_remove(buddy_link);
      if ((index & 1) == 0)
        address = link;
      else
        address = buddy_link;
    } else {
      stop = 1;
    }
    level--;
  } while (!stop);
}

static uint64_t list_free_space(uint8_t level) {
  list_t *list = &allLists[level];
  size_t size = SIZE_OF_BLOCKS_AT_LEVEL(level, HEAP_SIZE);

  uint64_t freeMem = 0;
  uint8_t actual = 0;

  list = list->next;
  while (list != NULL) {
    actual++;
    list = list->next;
  }

  freeMem = size * actual;
  return freeMem;
}

void memoryDump() {
  uint32_t index = 0;
  uint32_t freeSpace = 0;
  list_t *following;
  list_t *list;

  ncPrint("Memory dump - Buddy :");
  ncNewline();
  ncNewline();
  // printf("Memory dump:\n\n");

  for (int i = 0; i < levels; i++) {
    list = &allLists[i];

    if (!list_is_empty(list)) {
      // printf("\tBlock size: %d\n", SIZE_OF_BLOCKS_AT_LEVEL(i, HEAP_SIZE));
      ncPrint("     Block size: ");
      ncPrintDec(SIZE_OF_BLOCKS_AT_LEVEL(i, HEAP_SIZE));
      ncNewline();
      int sum = 0;
      for (following = list->next, index = 0; following != NULL;
           index++, following = following->next) {
        sum++;
      }
      // printf("\tLevel: %d - N° of blocks: %d\n", i, sum);
      ncPrint("     Level: ");
      ncPrintDec(i);
      ncPrint(" - No. of blocks: ");
      ncPrintDec(sum);
      ncNewline();

      freeSpace += list_free_space(i);
    }
  }
  ncPrint("Free memory space: ");
  ncPrintDec(freeSpace);
  ncNewline();
}

//---------------------------------------------

static void list_init(list_t *list) { list->prev = list->next = NULL; }

static list_t *belongs_to_list(list_t *buddy, uint8_t level) {
  list_t *aux = &allLists[level];
  aux = aux->next;
  while (aux != NULL) {
    if (aux == buddy) {
      return aux;
    }
    aux = aux->next;
  }
  return NULL;
}

static int list_is_empty(list_t *list) { return list->next == NULL; }

static list_t *list_pop(list_t *list) {
  list_t *curr = list;
  while (curr->next != NULL) curr = curr->next;
  list_remove(curr);
  return curr;
}

static void list_push(list_t *list, list_t *entry) {
  list_t *curr = list;
  while (curr->next != NULL) curr = curr->next;
  curr->next = entry;
  entry->next = NULL;
  entry->prev = curr;
}

static void list_remove(list_t *entry) {
  list_t *prev = entry->prev;
  list_t *next = entry->next;
  if (prev != NULL) prev->next = next;
  if (next != NULL) next->prev = prev;
}

// -------------------------------------

static int log_2(size_t n) {
  if (n == 0) {
    return -1;
  }
  int logValue = -1;
  while (n) {
    logValue++;
    n >>= 1;
  }
  return logValue;
}

static uint8_t getLevel(size_t size) {
  int level = 0;

  int currSize = HEAP_SIZE;

  if (size > currSize) return -1;
  while (size <= (currSize >> 1) && level < MAX_LEVELS - 1) {
    level++;
    currSize >>= 1;
  }

  return level;
}

static int getFirstAvailableLevel(int minLevel) {
  int selectedLevel;

  for (selectedLevel = minLevel; list_is_empty(&allLists[selectedLevel]);
       selectedLevel--)
    ;

  if (selectedLevel < 0) {
    return -1;
  }

  return selectedLevel;
}

static void addToLevel(list_t *list, list_t *node, uint8_t level) {
  node->level = level;
  list_push(list, node);
}
#endif
