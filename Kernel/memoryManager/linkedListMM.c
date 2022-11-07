// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// This is a personal academic project. Dear PVS-Studio, please check it.
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#ifndef BUDDY

#include "../include/MemoryManager.h"
#include "../include/lib.h"

#define WORD_ALIGN_DOWN(value) ((value) & (~(size_t)0x07))
#define WORD_ALIGN_UP(value) (WORD_ALIGN_DOWN((size_t)(value) + 7))

typedef struct memoryBlockNode {
  size_t size, leftoverSize;
  struct memoryBlockNode *previous, *next;
  size_t checksum;
} MemoryNode;

static size_t totalMemory;
static size_t usedMemory;
static unsigned int memoryChunks;

static MemoryNode* firstBlock = NULL;

static void calcNodeChecksum(const MemoryNode* node, size_t* result) {
  *result = node->size ^ node->leftoverSize ^ (size_t)node->previous ^
            (size_t)node->next;
}

void createMemoryManager() {
  void* actualStart = (void*)WORD_ALIGN_UP(TOTAL_HEAP_SIZE);
  size_t memorySize = TOTAL_MEMORY - (size_t)actualStart;
  memorySize = WORD_ALIGN_DOWN(memorySize);

  totalMemory = memorySize;
  usedMemory = sizeof(MemoryNode);
  memoryChunks = 1;

  firstBlock = (MemoryNode*)actualStart;
  memorySize -= sizeof(MemoryNode);

  firstBlock->size = 0;
  firstBlock->leftoverSize = memorySize;
  firstBlock->previous = NULL;
  firstBlock->next = NULL;
  calcNodeChecksum(firstBlock, &firstBlock->checksum);
}

void* allocMemory(size_t size) {
  if (firstBlock == NULL || size == 0) return NULL;

  size = WORD_ALIGN_UP(size);

  MemoryNode* node = firstBlock;
  size_t totalSizeWithNode = size + sizeof(MemoryNode);

  while ((node->size != 0 || node->leftoverSize < size) &&
         node->leftoverSize < totalSizeWithNode) {
    node = node->next;

    if (node == NULL) return NULL;
  }

  if (node->size == 0) {
    node->size = size;
    node->leftoverSize -= size;
    calcNodeChecksum(node, &node->checksum);
    usedMemory += size;
    return (void*)node + sizeof(MemoryNode);
  }

  MemoryNode* newNode =
      (MemoryNode*)((void*)node + sizeof(MemoryNode) + node->size);
  newNode->size = size;
  newNode->leftoverSize =
      node->leftoverSize - sizeof(MemoryNode) - newNode->size;
  newNode->previous = node;
  newNode->next = node->next;
  node->leftoverSize = 0;
  node->next = newNode;

  if (newNode->next != NULL) {
    newNode->next->previous = newNode;
    calcNodeChecksum(newNode->next, &newNode->next->checksum);
  }

  calcNodeChecksum(newNode, &newNode->checksum);
  calcNodeChecksum(node, &node->checksum);

  memoryChunks++;
  usedMemory += totalSizeWithNode;
  return (void*)newNode + sizeof(MemoryNode);
}

void freeMemory(void* address) {
  if (address == NULL) return;

  MemoryNode* node = (MemoryNode*)(address - sizeof(MemoryNode));

  size_t checksum;
  calcNodeChecksum(node, &checksum);
  if (checksum != node->checksum) return;

  if (node->previous == NULL) {
    node->leftoverSize += node->size;
    node->size = 0;
    usedMemory -= node->size;
    calcNodeChecksum(node, &node->checksum);
  } else {
    node->previous->leftoverSize +=
        node->size + node->leftoverSize + sizeof(MemoryNode);
    node->previous->next = node->next;
    usedMemory -= node->size + sizeof(MemoryNode);
    memoryChunks--;
    calcNodeChecksum(node->previous, &node->previous->checksum);

    if (node->next != NULL) {
      node->next->previous = node->previous;
      calcNodeChecksum(node->next, &node->next->checksum);
    }
  }
}

void memoryDump() {
  ncPrint("Memory Manager - Doubly Linked List \n");
  ncPrint("Total memory: ");
  ncPrintDec(totalMemory);
  ncNewline();
  ncPrint("Used memory: ");
  ncPrintDec(usedMemory);
  ncNewline();
  ncPrint("Chunks: ");
  ncPrintDec(memoryChunks);
  ncNewline();
  return;
}
#endif
